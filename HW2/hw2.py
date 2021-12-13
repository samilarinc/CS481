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
                    failure_node = self.search_word(chars)
                    if failure_node != None:
                        curr.fail = failure_node
                        break
                    chars.popleft()
                print(f"char:\t{curr.char}\tid:{curr.id}\tnext states:\t{[i.id for i in curr.next]}\tfail state:\t{curr.fail}\toutput: {curr.out}")
                
    def search_word(self, word):
        curr = self.root
        i = 0
        while len(curr.next) != 0 and len(word) > i:
            child = curr.has_child(word[i])
            if child == None: return None
            curr = child
            i += 1
        return curr

f = open('in', 'r')

patterns = f.readline().strip().split(' ')
sentence = f.readline().strip()
root = node()

my_tree = tree(root)
my_tree.build_tree(patterns)
my_tree.link_failure()

print("\nSearch\n" + "-"*70)

for i in range(len(sentence)):
    pass