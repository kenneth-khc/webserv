/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Initializer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:28:50 by kecheong          #+#    #+#             */
/*   Updated: 2025/03/21 17:40:19 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_INITIALIZER_HPP
#define VECTOR_INITIALIZER_HPP

/* Because C++98 doesn't support initializer lists, it gets troublesome to
 * construct an initialized vector. There are 2 options:
 * - Default construct a vector, then push_back the elements individually.
 *   eg:
 *		std::vector<int> v;
 *		v.push_back(1);
 *		v.push_back(2);
 *		v.push_back(3);
 *
 * - Create an array with the elements, then construct the vector by iterating
 *   through the array.
 *   eg:
 *		int numbers[5] = {1, 2, 3, 4, 5};
 *		std::vector<int> v(numbers, numbers+sizeof(numbers)/sizeof(*numbers));
 * 
 * Both are troublesome and ugly, so enter a 3rd (hacky) option:
 * - A VectorInitializer<T> constructs a vector<T>, then overloads operator()
 *   to push_back into the vector, allowing it to be chained for as many
 *   elements as necessary, and finally returning the vector<T>
 *   eg:
 *		std::vector<int> v = vector_of<int>(5)(10)(15)(20)(25);
 */

#include <vector>

template <typename E>
class	VectorInitializer
{
public:
	VectorInitializer();

	/* accepts an element in () and pushes back into the vector */
	VectorInitializer<E>&	operator()(const E&);

	/* implicitly convert into a vector<E> */
	operator std::vector<E>() const;

private:
	std::vector<E>	vector;
};

template <typename E>
VectorInitializer<E>::VectorInitializer():
	vector() { }

/* initializes a vector with the first element within the (),
   then continues chaining the rest of the ()s */
template <typename E>
VectorInitializer<E>	vector_of(const E& first)
{
	VectorInitializer<E>	initializer;
	initializer.operator()(first);
	return initializer;
}

template <typename E>
VectorInitializer<E>	vector_of()
{
	return VectorInitializer<E>();
}

template <typename E>
VectorInitializer<E>&
VectorInitializer<E>::operator()(const E& element)
{
	vector.push_back(element);
	return *this;
}

template <typename E>
VectorInitializer<E>::operator std::vector<E>() const
{
	return vector;
}

#endif
