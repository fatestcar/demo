from __future__ import division

import cv2
import numpy as np


class LaneDetector:
    def __init__(self, road_horizon, prob_hough=True):
        self.prob_hough = prob_hough
        self.vote = 70
        self.roi_theta = 0.4  # 这个是取到大于这个角的线
        self.road_horizon = road_horizon

    # def _standard_hough(self, img, init_vote):
    #     # 霍夫，原方法，未被调用
    #     lines = cv2.HoughLines(img, 1, np.pi / 180, init_vote)
    #     points = [[]]
    #     for l in lines:
    #         # 极坐标
    #         for rho, theta in l:
    #             a = np.cos(theta)
    #             b = np.sin(theta)
    #             x0 = a * rho
    #             y0 = b * rho
    #             x1 = int(x0 + 1000 * (-b))
    #             y1 = int(y0 + 1000 * a)
    #             x2 = int(x0 - 1000 * (-b))
    #             y2 = int(y0 - 1000 * a)
    #             points[0].append((x1, y1, x2, y2))
    #     return points

    def _base_distance(self, x1, y1, x2, y2, width):
        if x2 == x1:
            return (width * 0.5) - x1
        m = (y2 - y1) / (x2 - x1)
        c = y1 - m * x1
        base_cross = -c / m
        return (width * 0.5) - base_cross

    def _scale_line(self, x1, y1, x2, y2, frame_height):
        if x1 == x2:
            if y1 < y2:
                y1 = self.road_horizon
                y2 = frame_height
                return x1, y1, x2, y2
            else:
                y2 = self.road_horizon
                y1 = frame_height
                return x1, y1, x2, y2
        if y1 < y2:
            m = (y1 - y2) / (x1 - x2)
            x1 = ((self.road_horizon - y1) / m) + x1
            y1 = self.road_horizon
            x2 = ((frame_height - y2) / m) + x2
            y2 = frame_height
        else:
            m = (y2 - y1) / (x2 - x1)
            x2 = ((self.road_horizon - y2) / m) + x2
            y2 = self.road_horizon
            x1 = ((frame_height - y1) / m) + x1
            y1 = frame_height
        return x1, y1, x2, y2

    def detect(self, frame):
        img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        roiy_end = frame.shape[0]
        roix_end = frame.shape[1]
        roi = img[self.road_horizon:roiy_end, 0:roix_end]
        blur = cv2.medianBlur(roi, 5)
        contours = cv2.Canny(blur, 80, 150)

        lines = cv2.HoughLinesP(contours, 1, np.pi / 180, self.vote, minLineLength=60, maxLineGap=120)
        #     lines = self.standard_hough(contours, self.vote)

        if lines is not None:
            # 找到接近中心的线
            lines = lines + np.array([0, self.road_horizon, 0, self.road_horizon]).reshape(
                (1, 1, 4))  # 转换坐标
            left_bound = None
            right_bound = None
            for l in lines:
                # 找到左边最右的线和右边最左的线
                for x1, y1, x2, y2 in l:
                    theta = np.abs(np.arctan2((y2 - y1), (x2 - x1)))
                    if theta > self.roi_theta:
                        dist = self._base_distance(x1, y1, x2, y2, frame.shape[1])
                        if left_bound is None and dist < 0:
                            left_bound = (x1, y1, x2, y2)
                            left_dist = dist
                        elif right_bound is None and dist > 0:
                            right_bound = (x1, y1, x2, y2)
                            right_dist = dist
                        elif left_bound is not None and 0 > dist > left_dist:
                            left_bound = (x1, y1, x2, y2)
                            left_dist = dist
                        elif right_bound is not None and 0 < dist < right_dist:
                            right_bound = (x1, y1, x2, y2)
                            right_dist = dist
            if left_bound is not None:
                left_bound = self._scale_line(left_bound[0], left_bound[1], left_bound[2], left_bound[3],
                                              frame.shape[0])
            if right_bound is not None:
                right_bound = self._scale_line(right_bound[0], right_bound[1], right_bound[2], right_bound[3],
                                               frame.shape[0])

            return [left_bound, right_bound]
