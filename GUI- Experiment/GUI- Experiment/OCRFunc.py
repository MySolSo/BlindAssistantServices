import pytesseract
from PIL import Image

pytesseract.pytesseract.tesseract_cmd = 'C:/Program Files (x86)/Tesseract-OCR/tesseract'


def processImage(path):
    im = Image.open(path)
    text = pytesseract.image_to_string(im)
    return text



