# Configuration file reference

## Required
| Directive | Syntax | Default | Context | Description |
|:----------|:-------|:--------|:--------|:------------|
| prefix | prefix *absolute_path*; | — | global | Sets the base directory for the web server's files. Every relative path specified within the configuration are resolved relative to this. **Required.** |
| http | http { ... } | — | global  | Provides the context for HTTP settings. All HTTP related directives and server definitions go in here. **Required.** |

## HTTP configuration
| Directive | Syntax | Default | Context | Description |
|:----------|:-------|:--------|:--------|:------------|
| server | server { ... } | — | http | Defines a virtual host. Multiple server blocks can handle different domains, IP addresses or ports. The name server could be misleading, think of it as virtual hosts instead. |
| types | types *file*; | types mime.types | http | Specifies the MIME types configuration file. Mappings for file extensions to MIME types are retrieved from here. |

## Server configuration
| Directive | Syntax | Default | Context | Description |
|:----------|:-------|:--------|:--------|:------------|
| server_name | server_name *name* ...; | — | server | Sets the names of a virtual host. Incoming requests for the same endpoints are routed based on the Host header matching these names. |
| listen | listen *address:port*;  | listen 0.0.0.0:8000 | server | Sets the IP address and port to accept connections on. |
| location | location *uri* { ... } | — | server | Provides the context for configurations to use depending on a request URI. |

## Location configuration
| Directive | Syntax | Default | Context | Description |
|:----------|:-------|:--------|:--------|:------------|
| root | root *path*; | root html | http, server, location | Sets the root directory for handling requests. This will be appended to the prefix and the URI of a request will be appended to this to construct the final file path. |
| index | index *file* ... ; | index index.html | http, server, location | Sets files that will be used as an index. Files will be tried from left to right. |
| autoindex | autoindex on\|off;| autoindex off | http, server, location | Turning this on will enable directory listings. |
| redirect | redirect *URI* *replacement*; | — | location | If the specified URI matches the request URI, the request URI is changed as specified in the replacement string, and that URI is returned to the user.
| allow_method | allow_method *HTTP method* ...; | — | location, cgi_script | Sets the HTTP methods that are allowed on incoming requests. Currently, this only supports GET, POST and DELETE. |
| client_max_body_size | client_max_body_size *size*; | client_max_body_size 1m | http, server, location | Sets the maximum allowed size of the client request body. |
| error_page | error_page *code* ... *file*; | — | server | Sets the file to respond with when a given error code should be returned. |
| upload_directory | upload_directory *directory*; | — | location, cgi_script | Sets the directory where uploaded files are to be stored. |

## CGI script configuration
| Directive | Syntax | Default | Context | Description |
|:----------|:-------|:--------|:--------|:------------|
| cgi_script | cgi_script *file_extension* ... { ... } | — | server | Configures CGI execution for specific file extensions. Files with these extensions will be executed as CGI scripts. |
| cgi_bin_directory | cgi_bin_directory *path*; | — | cgi_script | Sets the directory where executable CGI scripts are stored. |
| cgi_upload_directory | cgi_upload_directory *path*; | — | cgi_script | Sets the directory for where uploads through a CGI script are to be stored. |
