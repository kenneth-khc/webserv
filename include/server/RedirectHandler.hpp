#ifndef REDIRECT_HANDLER_HPP
#define REDIRECT_HANDLER_HPP

#include "Directive.hpp"
#include "Response.hpp"
#include "String.hpp"

class	RedirectHandler
{
public:
	RedirectHandler();
	/* Construct a RedirectHandler based on the redirect directive within
	   the location block */
	RedirectHandler(const Directive&);

	/* Whether a HTTP redirection should be performed */
	bool	shouldRedirect;

	/* Performs the redirection by setting the Location header
	   and stops all processing after */
	void	executeRedirection(Response&) const;

private:
	/* Redirect code to use, only 301 or 302 for now */
	int		redirectCode;

	/* URI to redirect the client to */
	String	redirectUri;
};

#endif
