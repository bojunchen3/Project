import mediapipe as mp
import cv2
import sys
import os

label_name = sys.argv[1]
num_samples = int(sys.argv[2])
save_path = 'test'
class_path = os.path.join(save_path, label_name) 

cap = cv2.VideoCapture(0)
mpHands = mp.solutions.hands
hands = mpHands.Hands(min_detection_confidence=0.5, min_tracking_confidence=0.5, max_num_hands=1)
start = False
count = 0
while True:
    ret, frame = cap.read()
    if count == num_samples:
        break
    frame = cv2.flip(frame, 1)
    imgRGB = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    result = hands.process(imgRGB)
    imgHeight = frame.shape[0]
    imgWidth = frame.shape[1]
    if result.multi_hand_landmarks:
        for handLms in result.multi_hand_landmarks:
            for i, lm in enumerate(handLms.landmark):
                if i == 9:
                    xPos = int(lm.x * imgWidth)
                    yPos = int(lm.y * imgHeight)
                    if (xPos+120<imgWidth) and (yPos-120>0) and (xPos-120>0) and (yPos+120<imgHeight):
                        if start:
                            image = frame[(yPos-120):(yPos+120), (xPos-120):(xPos+120)]
                            save_path = os.path.join(class_path, '{}.jpg'.format(count + 1))
                            cv2.imwrite(save_path, image)
                            count += 1
                    cv2.rectangle(frame, (xPos-120, yPos-120), (xPos+120, yPos+120), (0, 0, 255), 3, cv2.LINE_AA)
    font = cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(frame, "Collecting {}".format(count),
               (5, 50), font, 0.7, (0, 255, 255), 2, cv2.LINE_AA)
    cv2.imshow("image", frame)

    k = cv2.waitKey(10)
    if k == ord('a'):
        start = not start
    if k == ord(' '):
        break

print("\n{} image(s) saved to {}".format(count, class_path))
cap.release()
cv2.destroyAllWindows()