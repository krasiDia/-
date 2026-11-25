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

		LinkedList& push_tail(T value) {
			Node<T>* tmp = head;
			Node<T>* newEl = new Node<T>(value);
			if (!head) {
				head = newEl;
				return *this;
			}
			while (tmp->next) {
				tmp = tmp->next;
			}
			tmp->next = newEl;
			newEl->prev = tmp;
			return *this;
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

		LinkedList& push_head(T value) {
			Node<T>* newEl = new Node<T>(value);

			if (!head) {
				head = newEl;
				return *this;
			}
			
			head->prev = newEl;
			newEl->next = head;

			head = newEl; 
			return *this;
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

		LinkedList& pop_head() {
			if (!head) { return *this; }
			if (!head->next) {
				delete head;
				head = nullptr;
				return *this;
			}
			Node<T>* tmp = head;
			head = head->next;

			delete tmp;
			return *this;
		}

		LinkedList& pop_tail() {
			if (!head) { return *this; }
			if (!head->next) {
				delete head;
				head = nullptr;
				return *this;
			}
			Node<T>* tmp = head;
			while (tmp && tmp->next && tmp->next->next) {
				tmp = tmp->next;
			}
			Node<T>* tail = tmp->next;
			tmp->next = nullptr;
			delete tail;
			return *this;
		}

		LinkedList& delete_node(T value) {
			Node<T>* tmp = head;

			while (tmp) {
				Node<T>* next = tmp->next;

				if (tmp->value == value) {
					if (tmp->prev) {
						tmp->prev->next = tmp->next;
					}
					else {
						head = tmp->next; 
					}

					if (tmp->next) {
						tmp->next->prev = tmp->prev;
					}

					delete tmp;
				}

				tmp = next;
			}

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& os, const LinkedList& m) {
			Node<T>* tmp = m.head;
			int count = 0;
			while (tmp) {
				count++;
				if (count % 3 == 0) {
					os << tmp->value << " ";
				}
				else {
					os << tmp->value << "";
				}
				tmp = tmp->next;
			}
			return os;
		}

		LinkedList& operator=(LinkedList other) {
			std::swap(head, other.head);
			return *this;
		}


		const T& operator[](size_t index) const {
			Node<T>* tmp = head;
			size_t count = 0;
			while (tmp) {
				if (count == index) {
					return (tmp->value);
				}
				tmp = tmp->next;
				count++;
			}
		}

		T& operator[](size_t index) {
			Node<T>* tmp = head;
			size_t count = 0;
			while (tmp) {
				if (count == index) {
					return (tmp->value);
				}
				tmp = tmp->next;
				count++;
			}
		}

		~LinkedList() {
			while (head) {
				Node<T>* tmp = head;
				head = head->next;
				delete tmp;
			}
		}

		int size() const{
			Node<T>* tmp = head;
			int count = 0;
			while (tmp) {
				tmp = tmp->next;
				count++;
			}
			return count;
		}


		LinkedList& reverse() {
			Node<T>* tmp = head;
			Node<T>* new_head = nullptr;

			while (tmp) {
				Node<T>* next = tmp->next; 

				std::swap(tmp->next, tmp->prev); 

				new_head = tmp; 
				tmp = next; 
			}

			head = new_head;
			return *this;
		}


		LinkedList operator+(LinkedList<int>& B) {
			if ((*this).size() < B.size()) {
				while ((*this).size() < B.size()) {
					(*this).push_head(0);
				}
			}
			if ((*this).size() > B.size()) {
				while ((*this).size() < B.size()) {
					B.push_head(0);
				}
			}
			LinkedList<int> res;
			int surplus = 0;
			for (int i = B.size() - 1; i > -1 ; --i) {
				int summand = ((*this)[i] + B[i] + surplus) % 10;
				surplus = ((*this)[i] + B[i] + surplus) / 10;
				res.push_tail(summand);
			}
			return res.reverse();
		}

		LinkedList operator*(const LinkedList<int>& B) {
			LinkedList<int> res;
			for (int i = B.size() - 1; i > -1; --i) {
				int surplus = 0;
				LinkedList<int> resSummand;
				for (int j = B.size() - 1; j > -1; --j) {
					int summand = (((*this)[j] * B[i]) + surplus) % 10;
					surplus = (((*this)[j] * B[i]) + surplus) / 10;
					resSummand.push_tail(summand);
				}
				resSummand.push_tail(surplus);

				for (int k = 0; k < (B.size() - 1) - i; ++k) {
					resSummand.push_head(0);
				}
				res = res + resSummand.reverse();
			}
			return res;
		}



};

int main() {

	//2. Сложить и умножить два числа, представленных в виде списков.
	// Числа не содержат ведущих нулей, кроме самого числа 0. 
	// Проверить ответ на числах, превышающих максимальное значение int.
	// Реализовать оператор вывода на печать так, чтобы он демонстрировал корректность выполненных операций.

	LinkedList<int> A({ 3,5,7, 1,2,1, 5,9,4 });
	LinkedList<int> B({ 5,8,5, 7,6,9, 0,2,1 });
	LinkedList<int> C = A + B;
	LinkedList<int> D = A * B;
	std::cout << C;
	std::cout << "\n";
	std::cout << D;
}