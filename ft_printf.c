/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Juyeong Maing <jmaing@student.42seoul.kr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 00:21:30 by Juyeong Maing     #+#    #+#             */
/*   Updated: 2022/12/05 01:51:37 by Juyeong Maing    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>

static int	special(char type, va_list *ap);

int	ft_printf(const char *format, ...)
{
	va_list		ap;
	const char	*current;
	int			printed;

	current = format;
	printed = 0;
	va_start(ap, format);
	while (*current)
	{
		if (*current != '%')
		{
			write(STDOUT_FILENO, current, 1);
			current++;
			printed += 1;
		}
		else
		{
			current++;
			printed += special(*current, &ap);
			current++;
		}
	}
	va_end(ap);
	return (printed);
}

static int	print_number(char type, uintmax_t n)
{
	int			radix;
	const char	*base;
	int			result;

	if (!n)
		return (0);
	base = "0123456789abcdef";
	if (type == 'X')
		base = "0123456789ABCDEF";
	radix = 16;
	if (type == 'o')
		radix = 8;
	if (type == 'u')
		radix = 10;
	result = print_number(type, n / radix);
	result += write(STDOUT_FILENO, &base[n % radix], 1);
	return (result);
}

static int	special_di(va_list *ap)
{
	const int	n = va_arg(*ap, int);
	int			result;

	if (!n)
		return (write(STDOUT_FILENO, "0", 1));
	if (n >= 0)
		return (print_number('u', n));
	result = write(STDOUT_FILENO, "-", 1);
	result += print_number('u', (n / 10) * -1);
	result += write(STDOUT_FILENO, &"0123456789"[(n % 10) * -1], 1);
	return (result);
}

static int	special_ouxp(char type, va_list *ap)
{
	uintmax_t	n;
	int			result;

	if (type != 'p')
		n = va_arg(*ap, unsigned int);
	else
		n = va_arg(*ap, uintptr_t);
	result = 0;
	if (type == 'p')
		result += write(STDOUT_FILENO, "0x", 2);
	if (!n)
		return (result + write(STDOUT_FILENO, "0", 1));
	return (result + print_number(type, n));
}

static int	special(const char type, va_list *ap)
{
	char		c;
	const char	*s;
	size_t		length;

	if (type == 'd' || type == 'i')
		return (special_di(ap));
	if (type == 'o' || type == 'u' || type == 'x' || type == 'X' || type == 'p')
		return (special_ouxp(type, ap));
	if (type == 'c')
	{
		c = va_arg(*ap, int);
		return (write(STDOUT_FILENO, &c, 1));
	}
	if (type == 's')
	{
		s = va_arg(*ap, const char *);
		if (!s)
			return (write(STDOUT_FILENO, "(null)", 6));
		length = 0;
		while (s[length])
			length++;
		return (write(STDOUT_FILENO, s, length));
	}
	return (write(STDOUT_FILENO, "%", 1));
}
