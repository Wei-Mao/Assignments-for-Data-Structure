'''
   Disjoint Sets support the following operations:
   make_set(x): creates a singleton set {x}
   find(x): returns ID of the set containing x
   union(x, y): merges two sets containing x and y,respectively.
'''

class Disjoint_Sets_Element:
    def __init__(self, _parent = -1, _rank = 0):
        self.parent = _parent
        self.rank = _rank

class Disjoint_Sets:
    def __init__(self, _size):
        '''
        Implement the Make_Set to create singleton sets
        '''
        self.sets = [Disjoint_Sets_Element(i, 0) for i in range(_size)]
        self.size = _size
        self.num_of_sets = _size;

    def find(self, i):
        while i != self.sets[i].parent:
            i = self.sets[i].parent

        return i

    def merge(self, i, j):
        i_id = self.find(i);
        j_id = self.find(j);

        if i_id == j_id:
            return
        elif self.sets[i_id].rank < self.sets[j_id].rank:
            self.num_of_sets -= 1
            self.sets[i_id].parent = j_id
        else:
            self.num_of_sets -= 1
            self.sets[j_id].parent = i_id

            if self.sets[i_id].rank == self.sets[j_id].rank:
                self.sets[i_id].rank += 1

    def print_state(self):
        print("parent")
        for i in range(self.size):
            print(f'{self.sets[i].parent} ', end='')

        print('\n')


        print("rank")
        for i in range(self.size):
            print(f'{self.sets[i].rank} ', end='')

        print('\n')

if __name__ == '__main__':
    size = 6
    dis_sets = Disjoint_Sets(size)
    dis_sets.merge(1, 3);
    dis_sets.print_state();
    dis_sets.merge(4, 1);
    dis_sets.print_state();
    dis_sets.merge(2, 0);
    dis_sets.print_state();
    dis_sets.merge(1, 2);
    dis_sets.print_state();
    dis_sets.merge(1, 5);
    dis_sets.print_state();


