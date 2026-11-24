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
		LinkedList(std::initializer_list<T> values) {
			if (values.empty()) {
				return;
			}
			if (values.size() == 1) {
				head = new Node<T>(*values.begin());
				return;
			}
			head = new Node<T>(*values.begin());
			Node<T>* tmp = head;

			bool count = true;
			for (const T& value : values) {
				if (count) {
					count = false;
					continue;
				}
				Node<T>* newTmp = new Node<T>(value);
				tmp->next = newTmp;
				newTmp->prev = tmp;

				tmp = tmp->next;
			}
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
		LinkedList(int n, int top, int bottom) {
			//int seed = 45; //фиксированное зерно 
			int seed = std::chrono::steady_clock::now().time_since_epoch().count(); //нефиксированное зерно 

			//генератор псевдослучайных чисел 
			std::mt19937 gen(seed);

			if constexpr (std::is_integral_v<T>) {
				//равномерное распределение
				std::uniform_int_distribution<int> dist(top, bottom);
				for (int i = 0; i < n; i++) {
					push_tail(dist(gen));
				}
			}

			else if constexpr (std::is_floating_point_v<T>) {
				std::uniform_real_distribution<T> dist(
					static_cast<T>(top),
					static_cast<T>(bottom)
				);

				for (int i = 0; i < n; ++i) {
					push_tail(dist(gen));
				}
			}
			else if constexpr (std::is_same_v<T, std::complex<float>>) {
				std::uniform_real_distribution<float> dist(
					static_cast<float>(top),
					static_cast<float>(bottom)
				);

				for (int i = 0; i < n; ++i) {
					T value(dist(gen), dist(gen));
					push_tail(value);
				}
			}
			else if constexpr (std::is_same_v < T, std::complex<double>>) {
				std::uniform_real_distribution<double> dist(
					static_cast<double>(top),
					static_cast<double>(bottom)
				);

				for (int i = 0; i < n; ++i) {
					T value(dist(gen), dist(gen));
					push_tail(value);
				}
			}
		}

		bool push_tail(T value) {
			Node<T>* tmp = head;
			Node<T>* newEl = new Node<T>(value);
			if (!head) {
				head = newEl;
				return 0;
			}
			while (tmp->next) {
				tmp = tmp->next;
			}
			tmp->next = newEl;
			newEl->prev = tmp;
			return 0;
		}


		LinkedList& push_tail(LinkedList other) {
			if (this == &other) return *this;
			if (!other.head) { return *this; }
			if (!head) { 
				head = other.head; 
				other.head = nullptr;
				return *this; 
			}
			Node<T>* tmp = head;
			while (tmp && tmp->next) {
				tmp = tmp->next;
			}
			tmp->next = other.head;
			other.head->prev = tmp;

			other.head = nullptr; //обнуляем чтобы не появл пробл висячего указателя и UB
			return *this;
		}

		bool push_head(T value) {
			Node<T>* newEl = new Node<T>(value);

			if (!head) {
				head = newEl;
				return 0;
			}
			
			head->prev = newEl;
			newEl->next = head;

			head = newEl; 
			return 0;
		}

		LinkedList& push_head(LinkedList other) {
			if (this == &other) return *this;
			if (!other.head) { return *this; }
			if (!head) {
				head = other.head;
				other.head = nullptr;
				return *this;
			}
			other.push_tail(*this);
			head = other.head;
			other.head = nullptr;
			return *this;
		}


		friend std::ostream& operator<<(std::ostream& os, const LinkedList& m) {
			Node<T>* tmp = m.head;
			while (tmp) {
				os << tmp->value << " ";
				tmp = tmp->next;
			}
			return os;
		}

		LinkedList& operator=(LinkedList other) {
			std::swap(head, other.head);
			return *this;
		}

		~LinkedList() {
			while (head) {
				Node<T>* tmp = head;
				head = head->next;
				delete tmp;
			}
		}

};

int main() {
	LinkedList<int> A({ 1, 2, 3 });
	LinkedList<int> B(4, 1, 10);
	LinkedList<int> C(A);
	LinkedList<int> D;
	D = B;
	D.push_tail(174);
	D.push_tail(A);
	D.push_head(123);
	A.push_head(B);
	std::cout << B << "\n" << D << "\n" << A;
}