import pytesseract
import cv2
from PIL import Image
import os

def turnImageToAtr():
    imagePath = 'currentProcessedLetter.jpg'

    imageMat = cv2.imread(imagePath, cv2.cv2.IMREAD_COLOR)
    imageMat = cv2.resize(imageMat, (20, 20))
    cv2.imwrite(imagePath, imageMat)

    letter = Image.open(imagePath)
    letterToText = pytesseract.image_to_string(letter, config='-psm 10')

    os.remove(imagePath)

    return letterToText
