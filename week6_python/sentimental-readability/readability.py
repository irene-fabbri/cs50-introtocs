import cs50
import re

text = cs50.get_string("Text: ")

letters = len(re.findall('[a-zA-Z]', text))
words = len(text.split())
sentences = len(re.split('[.!?]', text))-1

#print(f"{letters} {words} {sentences}")
index = (0.0588 * letters - 0.296 * sentences) / words * 100 - 15.8

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")
