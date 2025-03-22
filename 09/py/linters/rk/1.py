with open("in.txt", "r") as f_in:
    with open("out.txt", "w") as f_out:
        current_sentence = ""

        for line in f_in:
            line = line.strip()

            if line == "":
                continue

            while True:
                if line == "":
                    break

                pos = line.find(".")
                if pos == -1:
                    current_sentence += line.strip() + " "
                    break

                current_sentence += line[:pos+1]
                f_out.write(current_sentence)
                f_out.write("\n")
                current_sentence = ""
                line = line[pos+1:].strip()
