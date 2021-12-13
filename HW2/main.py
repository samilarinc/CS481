from argparse import ArgumentParser

class node(object):
    def __init__(self):
        self.next = []
        self.fail = 0
        self.out = []
        self.char = ''
        self.id = 0
    
    def has_child(self, c):
        for i in self.next:
            if i.char == c:
                return i
        return None

    def __str__(self):
        return str(self.id)

class tree():
    def __init__(self, root_node):
        self.root = root_node
    
    def build_tree(self, patterns):
        node_id = 0
        for pat in patterns:
            curr = root
            for char in pat:
                child = curr.has_child(char)
                if child != None:
                    curr = child
                else:
                    new_node = node()
                    node_id += 1
                    new_node.char = char
                    new_node.id = node_id
                    curr.next.append(new_node)
                    curr = new_node
            curr.out.append(pat)
        
    def link_failure(self):
        from collections import deque
        prev_nodes = deque()
        prev_chars = deque()
        visited = list()
        curr = self.root
        print("Build tree\n" + "-"*70)
        print(f"char:\t \tnext states:\t{[i.id for i in curr.next]}\tfail state:\t0\toutput:\t[]")
        while True:
            rem = list()
            for i in curr.next: 
                if i not in visited: rem.append(i)
            if len(rem) == 0:
                if len(prev_nodes) == 0: break
                curr = prev_nodes.pop()
                prev_chars.pop()
            else:
                prev_nodes.append(curr)
                curr = rem[0]
                prev_chars.append(curr.char)
                visited.append(curr)
                chars = prev_chars.copy()
                while len(chars) > 1:
                    chars.popleft()
                    failure_node = self.search_word(chars, internal =True)
                    if failure_node != None:
                        curr.fail = failure_node
                        break
                    chars.popleft()
                print(f"char:\t{curr.char}\tnext states:\t{[i.id for i in curr.next]}\t\tfail state:\t{curr.fail}\toutput: {curr.out}")
                
    def search_word(self, word, internal = False):
        curr = self.root
        i = 0
        all_out = list()
        while len(curr.next) != 0 and len(word) > i:
            child = curr.has_child(word[i])
            if not internal and len(curr.out) > 0:
                all_out.append(curr.out[0])
            if child == None: 
                return None if internal else all_out
            curr = child
            i += 1
        if not internal:
            all_out.append(curr.out[0])
        return curr if internal else all_out

if __name__ == '__main__':
    print()
    parser = ArgumentParser(description="This program builds a tree of patterns using Aho-Corasick Algorithm and searchs these patterns into text")
    parser.add_argument('-i', "--input", required=True)
    args = parser.parse_args()
    f = open(args.input, 'r')
    
    patterns = f.readline().strip().split(' ')
    text = f.readline().strip()
    root = node()
    
    my_tree = tree(root)
    my_tree.build_tree(patterns)
    my_tree.link_failure()
    
    def text_search(my_tree, text):
        print("\nSearch\n" + "-"*70)
        index = 0
        while index < len(text):
            node_list = my_tree.search_word(text[index:])
            for i in node_list:
                print("keyword:", i, "index:", index)
            index += 1
            
    text_search(my_tree, text)
    print()