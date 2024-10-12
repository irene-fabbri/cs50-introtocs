def get_height(prompt):
    while True:
        try:
            while True:
                height = int(input(prompt))
                if height > 0 and height < 9:
                    return height
        except ValueError:
            pass


def main():
    answer = get_height("Height: ")
    for i in range(1, answer+1):
        print(" " * (answer - i) + "#" * i + "  " + "#" * i)


main()
