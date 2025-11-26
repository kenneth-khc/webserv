
#include "RedirectHandler.hpp"
#include "Directive.hpp"
#include "Optional.hpp"
#include "Parameter.hpp"
#include "Time.hpp"

RedirectHandler::RedirectHandler():
shouldRedirect(false),
redirectCode(0),
redirectUri("")
{
}

RedirectHandler::RedirectHandler(const Directive& location):
shouldRedirect(false),
redirectCode(0),
redirectUri("")
{
	Optional<Directive*>	redirect = location.getDirective("redirect");
	if (redirect.exists)
	{
		const std::vector<Parameter>	parameters = redirect.value->parameters;

		shouldRedirect = true;
		redirectCode = parameters[0].value.toInt();
		redirectUri = parameters[1].value;
	}
}

void	RedirectHandler::executeRedirection(Response& response) const
{
	response.setStatusCode(redirectCode);
	response.insert("Location", redirectUri);
	response.insert("Date", Time::printHTTPDate());
	response.insert("Content-Length", 0);
	response.processStage |= Response::DONE;
}
