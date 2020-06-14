#python3
import sys

class StackWithMax():
    def __init__(self):
        self.__stack = []
        self.__max_value = 0;

    def Push(self, a):
        if (len(self.__stack) == 0):
            self.__stack.append(a)
            self.__max_value = a
        else:
            if(a <= self.__max_value):
                self.__stack.append(a)
            else:
                self.__stack.append(2 * a - self.__max_value)
                self.__max_value = a



    def Pop(self):
        assert(len(self.__stack))
        pop_value = self.__stack[-1]
        self.__stack.pop()
        if(pop_value > self.__max_value):
            self.__max_value = 2 * self.__max_value - pop_value

    def Max(self):
        assert(len(self.__stack))
        return self.__max_value


if __name__ == '__main__':
    stack = StackWithMax()

    num_queries = int(sys.stdin.readline())
    for _ in range(num_queries):
        query = sys.stdin.readline().split()

        if query[0] == "push":
            stack.Push(int(query[1]))
        elif query[0] == "pop":
            stack.Pop()
        elif query[0] == "max":
            print(stack.Max())
        else:
            assert(0)
