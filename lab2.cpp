import std;

template <typename T>
struct Node {
	T value{};
	Node<T>* next;
	Node<T>* prev;

	Node(T value) : value(value), next(nullptr), prev(nullptr) {};
	Node(T value, Node<T>* nxt, Node<T>* prv) : value(value), next(nxt), prev(prv) {};
};

template <typename T>
class LinkedList {
	private: 
		Node<T>* head = nullptr;
	public:
		LinkedList() = default;
		LinkedList(Node<T>* tmp) {
			head = tmp;
		}
		LinkedList(const LinkedList<T>& other) {
			if (!other.head) return;

			Node<T>* tmpOther = other.head;

			head = new Node<T>(tmpOther->value);
			Node<T>* tmp = head;

			tmpOther = tmpOther->next;

			while (tmpOther) {
				Node<T>* newElement = new Node<T>(tmpOther->value);

				tmp->next = newElement;
				newElement->prev = tmp;

				tmp = newElement;
				tmpOther = tmpOther->next;
			}
		}

};

int main() {
	Node<int> B(6);
	LinkedList<int> A(&B);
	LinkedList<int> C(A);
}