/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Optional.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cteoh <cteoh@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 06:04:17 by kecheong          #+#    #+#             */
/*   Updated: 2025/02/05 20:20:57 by cteoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

/* A naive implementation of std::optional from C++17
 *
 * An Optional is a wrapper around an object that may or may not exist.
 * Useful in situations where an operation could either return a result or
 * return "nothing", and instead of using weird sentinel values like -1 or
 * NULL we return an Optional wrapping the result, with exists set to true or
 * false to indicate whether it contains the result or not.
 */

template <typename T>
class	Optional
{
public:
	Optional();
	Optional(const T&);
	Optional(const Optional&);
	~Optional();

	bool	exists;
	T		value;

	Optional&	operator=(const T&);
	Optional	operator()(const T&);
	operator	bool() const;

	void		reset();
	template <typename U>
	T	value_or(const U& defaultValue) const;

};

template <typename T>
Optional<T>::Optional():
exists(false),
value()
{

}

template <typename T>
Optional<T>::Optional(const T& thing):
exists(true),
value(thing)
{

}

template <typename T>
Optional<T>::Optional(const Optional& other):
exists(other.exists)
{
	if (other.exists)
	{
		value = other.value;
	}
}

template <typename T>
Optional<T>::~Optional() { }

template <typename T>
Optional<T>	Optional<T>::operator()(const T& thing)
{
	if (exists)
	{
		exists = false;
	}
	value = thing;
	exists = true;
}

template <typename T>
Optional<T>&	Optional<T>::operator=(const T& thing)
{
	if (exists)
	{
		exists = false;
	}
	value = thing;
	exists = true;
}

template <typename T>
Optional<T>::operator bool() const
{
	return exists;
}

template <typename T>
Optional<T>	makeOptional(T thing)
{
	return Optional<T>(thing);
}

template <typename T>
Optional<T>	makeNone()
{
	return Optional<T>();
}

template <typename T>
void	Optional<T>::reset()
{
	if (exists)
	{
		exists = false;
	}
}


template <typename T>
template <typename U>
T	Optional<T>::value_or(const U& defaultValue) const
{
	if (exists)
	{
		return value;
	}
	else
	{
		return static_cast<T>(defaultValue);
	}
}

#endif
