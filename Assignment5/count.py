def count_words_in_file(filename):
    try:
        with open(filename, 'r') as file:
            content = file.read()
            words = content.split()
            return len(words)
    except FileNotFoundError:
        print(f"文件 '{filename}' 未找到。")
        return None

filename = 'test.txt'  # 替换为你的文件名
word_count = count_words_in_file(filename)

if word_count is not None:
    print(f"文件 '{filename}' 中有 {word_count} 个单词。")
