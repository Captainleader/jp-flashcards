def classify_cards(file_path):
    total = 0
    new_count = 0
    learning_count = 0
    mastered_count = 0

    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line or not line.startswith("#"):
                continue

            parts = line.split('\t')
            if len(parts) < 6:
                continue

            total += 1
            level = int(parts[4])
            next_review = parts[5]

            if next_review == '0':
                new_count += 1
            elif level >= 10:
                mastered_count += 1
            else:
                learning_count += 1

    def percent(count):
        return (count / total) * 100 if total > 0 else 0

    print(f"Total cards: {total}")
    print(f"🆕 New: {new_count} ({percent(new_count):.1f}%)")
    print(f"📘 Learning: {learning_count} ({percent(learning_count):.1f}%)")
    print(f"🏆 Mastered: {mastered_count} ({percent(mastered_count):.1f}%)")

# Example usage:
classify_cards("card_data.tsv")
