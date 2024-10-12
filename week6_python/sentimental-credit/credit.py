import re
cards = [
    {"name": "AMEX", "format": r"^[3][47]\d{13}$"},
    {"name": "MASTERCARD", "format": r"^[5][1-5]\d{14}$"},
    {"name": "VISA", "format": r"^4\d{12}$"},
    {"name": "VISA", "format": r"^4\d{15}$"},
]


def main():
    credit_num = input("Number: ")
    if (checksum(credit_num)):
        for card in cards:
            if re.match(card["format"], credit_num):
                print(card["name"])
                return
        else:
            print("INVALID")
            return
    else:
        print("INVALID")
        return


def double_split(ch):
    return str(int(ch)*2)


def checksum(credit):
    if credit.isdigit() and int(credit) > 0:
        start = len(credit) % 2
        to_double = list(''.join(map(double_split, credit[start::2])))
        doubled_num = list(map(int, to_double))
        to_copy = list(credit[1-start::2])
        copied_num = list(map(int, to_copy))
        if (sum(doubled_num)+sum(copied_num)) % 10 == 0:
            return True
        else:
            return False
    else:
        return False


main()
