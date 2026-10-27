import std;
#include <cstdarg>

//структура для связанного списка
template<typename T>
struct Array {
	T value{};
	Array<T>* next = nullptr;
};

//добавление в связанный список
template<typename T>
void add(Array<T>*& head, T value) {
	Array<T>* tmp = head;
	Array<T>* newEl = new Array<T>(value, nullptr);
	if (!head) {
		head = newEl;
		return;
	}
	while (tmp->next) {
		tmp = tmp->next;
	}
	tmp->next = newEl;
}


template<typename T>
class Matrix {
	Array<T>* arrayMatrix = nullptr;
	int row{}, col{};
	static const float deff;
public:
	Matrix() = default;
	Matrix(int row, int col, std::initializer_list<T> values): row(row), col(col){
		for (const T& value : values) {
			add(arrayMatrix, value);
		}
	}
	Matrix(int row, int col, int top, int bottom): row(row), col(col) {
		std::random_device r;
		std::default_random_engine e1(r());
		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<int> dist(top, bottom);

			for (int i = 0; i < row * col; ++i) {
				add(arrayMatrix, static_cast<T>(dist(e1)));
			}
		}
		else if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> dist(
				static_cast<T>(top),
				static_cast<T>(bottom)
			);

			for (int i = 0; i < row * col; ++i) {
				add(arrayMatrix, dist(e1));
			}
		}
		else if constexpr (std::is_same_v<T, std::complex<float>>) {
			std::uniform_real_distribution<float> dist(
				static_cast<float>(top),
				static_cast<float>(bottom)
			);

			for (int i = 0; i < row * col; ++i) {
				T value(dist(e1), dist(e1));
				add(arrayMatrix, value);
			}
		}
		else if constexpr (std::is_same_v < T, std::complex<double>>) {
			std::uniform_real_distribution<double> dist(
				static_cast<double>(top),
				static_cast<double>(bottom)
			);

			for (int i = 0; i < row * col; ++i) {
				T value(dist(e1), dist(e1));
				add(arrayMatrix, value);
			}
		}
	}
	Matrix(const Matrix& m) : row(m.row), col(m.col) {
			Array<T>* tmp = m.arrayMatrix;
			while (tmp) {
				add(arrayMatrix, tmp->value);
				tmp = tmp->next;
			}
	}
	Matrix& operator=(Matrix other) {
			std::swap(arrayMatrix, other.arrayMatrix);
			std::swap(row, other.row);
			std::swap(col, other.col);
			return *this;
	}

	bool operator==(const Matrix& m) const{
		Array<T>* tmp = this->arrayMatrix;
		Array<T>* tmpOther = m.arrayMatrix;
		while (tmp && tmpOther) {
			if constexpr (std::is_integral_v<T>) {
				if (tmp->value != tmpOther->value) {
					return false;
				}
			}
			else if constexpr (
				std::is_floating_point_v<T> ||
				std::is_same_v<T, std::complex<float>> ||
				std::is_same_v<T, std::complex<double>>
				) {
				double eps = 1e-6;

				if (std::abs(tmp->value - tmpOther->value) > eps) {
					return false;
				}
			}
			tmp = tmp->next;
			tmpOther = tmpOther->next;
		}
		return tmp == nullptr && tmpOther == nullptr;
	}
	bool operator!=(const Matrix& m) const{
		return !(*this == m);
	}
	friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
		Array<T>* tmp = m.arrayMatrix;
		while (tmp) {
			for (int i = 0; i < m.col; ++i) {
				os << tmp->value << " ";
				tmp = tmp->next;
			}
			os << "\n";
		}
		return os;
	}
	T operator()(int indexRow, int indexCol) const{
		if (indexRow < 0 || indexRow >= this->row ||
			indexCol < 0 || indexCol >= this->col) {
			throw std::runtime_error("Incorrect index 1");
		}

		// Вычисляем позицию элемента в списке
		int position = indexRow * this->col + indexCol;

		Array<T>* tmp = this->arrayMatrix;

		for (int i = 0; i < position; ++i) {
			tmp = tmp->next;
		}
		return tmp->value;
	}
	void operator()(int indexRow, int indexCol, T mean){
		if (indexRow < 0 || indexRow >= this->row ||
			indexCol < 0 || indexCol >= this->col) {
			throw std::runtime_error("Incorrect index 2");
		}

		int position = indexRow * this->col + indexCol;

		Array<T>* tmp = this->arrayMatrix;

		for (int i = 0; i < position; ++i) {
			tmp = tmp->next;
		}
		tmp->value = mean;
	}

	void operator+=(const Matrix<T>& other) {
		if (this->row == other.row && this->col == other.col) {
			Array<T>* tmp = this->arrayMatrix;
			Array<T>* tmpOther = other.arrayMatrix;
			while (tmp && tmpOther) {
				tmp->value += tmpOther->value;
				tmp = tmp->next;
				tmpOther = tmpOther->next;
			}
		}
		else {
			throw std::runtime_error("The matrices have different sizes.");
		}
	}
	friend Matrix<T> operator+(const Matrix<T>& m, const Matrix<T>& o) {
		Matrix<T> tmp = m;
		tmp += o;
		return tmp;
	}

	void operator-=(const Matrix<T>& other) {
		if (this->row == other.row && this->col == other.col) {
			Array<T>* tmp = this->arrayMatrix;
			Array<T>* tmpOther = other.arrayMatrix;
			while (tmp && tmpOther) {
				tmp->value -= tmpOther->value;
				tmp = tmp->next;
				tmpOther = tmpOther->next;
			}
		}
		else {
			throw std::runtime_error("The matrices have different sizes.");
		}
	}
	friend Matrix<T> operator-(const Matrix<T>& m, const Matrix<T>& o) {
		Matrix<T> tmp = m;
		tmp -= o;
		return tmp;
	}

	void operator*=(const T& mean) {
		Array<T>* tmp = this->arrayMatrix;
		while (tmp) {
			tmp->value *= mean;
			tmp = tmp->next;
		}
	}
	friend Matrix<T> operator*(const Matrix<T>& m, const T& mean) {
		Matrix<T> ans = m;
		ans *= mean;
		return ans;
	}
	friend Matrix<T> operator*(const T& mean, const Matrix<T>& m) {
		Matrix<T> ans = m;
		ans *= mean;
		return ans;
	}

	void operator/=(const T& mean) {
		if (mean == T{}) {
			throw std::runtime_error("Division by zero.");
		}
		Array<T>* tmp = this->arrayMatrix;
		while (tmp) {
			tmp->value /= mean;
			tmp = tmp->next;
		}
	}
	friend Matrix<T> operator/(const Matrix<T>& m, const T& mean) {
		Matrix<T> ans = m;
		ans /= mean;
		return ans;
	}

	T tr() const {
		if (this->row == this->col) {
			T result{};
			for (int i = 0; i < this->row; ++i) {
				result += (*this)(i,i);
			}
			return result;
		}
		else {
			throw std::runtime_error("The matrix must be square");
		}
	}
	friend Matrix<T> operator*(const Matrix<T>& a, const Matrix<T>& other) {
		if (a.col == other.row) {
			Matrix<T> tmp(a.row, other.col, 0, 0);
			for (int k = 0; k < a.row; ++k) {
				for (int i = 0; i < other.col; ++i) {
					T result{};
					for (int j = 0; j < a.col; ++j) {
						result += a(k,j) * other(j,i);
					}
					tmp(k, i, result);
				}
			}
			return tmp;
		}
		else {
			throw std::runtime_error("The matrices are not adjacent.");
		}
	}
	~Matrix() {
		while (arrayMatrix) {
			Array<T>* tmp = arrayMatrix;
			arrayMatrix = arrayMatrix->next;
			delete tmp;
		}
	}
};

template<typename T>
T determinant(const Matrix<T>& A) {
	return A(0, 0) * (A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1))
		- A(0, 1) * (A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0))
		+ A(0, 2) * (A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0));
}
template<typename T>
Matrix<T> inverse(const Matrix<T>& A) {
	T det = determinant(A);

	if (std::abs(det) < 1e-6) {
		throw std::runtime_error("Inverse matrix does not exist");
	}

	Matrix<T> result(3, 3, {
		A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1),
		A(0, 2) * A(2, 1) - A(0, 1) * A(2, 2),
		A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1),

		A(1, 2) * A(2, 0) - A(1, 0) * A(2, 2),
		A(0, 0) * A(2, 2) - A(0, 2) * A(2, 0),
		A(0, 2) * A(1, 0) - A(0, 0) * A(1, 2),

		A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0),
		A(0, 1) * A(2, 0) - A(0, 0) * A(2, 1),
		A(0, 0) * A(1, 1) - A(0, 1) * A(1, 0)
		});

	return result / det;
}
template<typename T>
Matrix<T> solve(const Matrix<T>& A, const Matrix<T>& b) {
	return inverse(A) * b;
}
template<typename T>
bool areCoplanar(
	const Matrix<T>& a,
	const Matrix<T>& b,
	const Matrix<T>& c
) {
	T det =
		a(0, 0) * (b(0, 1) * c(0, 2) - b(0, 2) * c(0, 1))
		- a(0, 1) * (b(0, 0) * c(0, 2) - b(0, 2) * c(0, 0))
		+ a(0, 2) * (b(0, 0) * c(0, 1) - b(0, 1) * c(0, 0));

	return std::abs(det) < 1e-6;
}

template<typename T>
std::pair<Matrix<T>, Matrix<T>> LU(const Matrix<T>& A) {
	Matrix<T> L(3, 3, 0, 0);
	Matrix<T> U(3, 3, 0, 0);

	for (int i = 0; i < 3; ++i) {
		L(i, i, T{ 1 });

		//столбцы
		for (int j = i; j < 3; ++j) {
			T sum{};
			//строки
			for (int k = 0; k < i; ++k) {
				sum += L(i, k) * U(k, j);
			}

			U(i, j, A(i, j) - sum);
		}

		for (int j = i + 1; j < 3; ++j) {
			T sum{};

			for (int k = 0; k < i; ++k) {
				sum += L(j, k) * U(k, i);
			}

			if (std::abs(U(i, i)) < 1e-6) {
				throw std::runtime_error("LU decomposition failed");
			}

			L(j, i, (A(j, i) - sum) / U(i, i));
		}
	}

	return { L, U };
}

template<typename T>
Matrix<T> power(const Matrix<T>& A, int k) {
	if (k < 0) {
		throw std::runtime_error("Power must be non-negative");
	}

	Matrix<T> result(3, 3, 0, 0);

	for (int i = 0; i < 3; ++i) {
		result(i, i, T{ 1 });
	}

	for (int i = 0; i < k; ++i) {
		result = result * A;
	}

	return result;
}

int main() {
	try {
		//1-2
		{
			Matrix<double> A(3, 3, { 2.0, 1.0, -1.0, -3.0, -1.0, 2.0, -2.0, 1.0, 2.0 });

			Matrix<double> b(3, 1, { 8.0, -11.0, -3.0 });

			Matrix<double> x = solve(A, b);
			std::cout << "Task 1: ";
			std::cout << "x:\n" << x << "\n";
		}

		//3
		{
			Matrix<double> a(1, 3, { 1.0, 2.0, 3.0 });
			Matrix<double> b(1, 3, { 4.0, 5.0, 6.0 });
			Matrix<double> c(1, 3, { 7.0, 8.0, 9.0 });
			std::cout << "Task 3: ";
			std::cout << std::boolalpha << areCoplanar(a, b, c) << "\n";
			std::cout << "\n";
		}

		//4
		{
			Matrix<double> A(3, 3, {2.0, 1.0, 1.0, 4.0, 3.0, 3.0, 8.0, 7.0, 9.0});

			auto [L, U] = LU(A);
			std::cout << "Task 4: ";
			std::cout << "L:\n" << L;
			std::cout << "U:\n" << U;
			std::cout << "L * U:\n" << L * U;
			std::cout << "\n";
		}
		//5
		{
			Matrix<double> A(3, 3, {
					1.0, 2.0, 0.0,
					0.0, 1.0, 3.0,
					0.0, 0.0, 1.0
				});

			Matrix<double> result = power(A, 3);

			std::cout << "Task 5: " << "\n" << result;
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
	}
}