
import cv2
import track
import detect


def main(video_path):
    print("here")
    # cap = cv2.VideoCapture(video_path)
    # ticks = 0
    #
    # lt = track.LaneTracker(2, 0.1, 150)
    # ld = detect.LaneDetector(120)
    # while cap.isOpened():
    #     prec_tick = ticks
    #     ticks = cv2.getTickCount()
    #     dt = (ticks - prec_tick) / cv2.getTickFrequency()
    #     ret, frame = cap.read()
    #     predicted = lt.predict(dt)
    #     lanes = ld.detect(frame)
    #     if predicted is not None:
    #         cv2.line(frame, (predicted[0][0], predicted[0][1]), (predicted[0][2], predicted[0][3]), (0, 0, 255), 5)
    #         cv2.line(frame, (predicted[1][0], predicted[1][1]), (predicted[1][2], predicted[1][3]), (0, 0, 255), 5)
    #
    #     lt.update(lanes)
    #     cv2.imshow('', frame)
    #     if cv2.waitKey(1) & 0xFF == ord('q'):
    #         break

# if __name__ == '__main__':
#     main('video.avi')