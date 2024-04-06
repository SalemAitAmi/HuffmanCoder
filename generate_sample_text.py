import random

def generate_random_text():
    # Frequencies of characters (sample DNA data)
    char_frequencies = {'A': 50000, 'C': 30000, 'T': 20000, 'G': 25000}

    # Generate characters based on frequencies
    text = ''.join(random.choices(list(char_frequencies.keys()), 
                                  weights=list(char_frequencies.values()), 
                                  k=sum(char_frequencies.values())))

    return text

if __name__ == "__main__":
    random_text = generate_random_text()
    with open("random_text.txt", "w") as file:
        file.write(random_text)

    print("Random text generated and saved in random_text.txt.")