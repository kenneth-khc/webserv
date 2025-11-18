# Configuration file reference

## Required
| Directive | Syntax                 | Default | Context | Description |
|:---------:|:-----------------------|:-------:|:-------:|:------------|
| prefix    | prefix *absolute_path* |    —    | global  | Sets the base directory for the web server's files. Every relative path specified within the configuration will be appended to this. **Required.** |
| http      | http { ... }           |    —    | global  | Provides the context for HTTP settings. All HTTP related directives go in here. **Required.** |

## HTTP configuration
| Directive | Syntax                 | Default          | Context                | Description |
|:---------:|:-----------------------|:----------------:|:----------------------:|:------------|
| server    | server { ... }         |    —             | http                   | Provides the context for configurating a virtual server. |
| root      | root *path*;           | root html        | http, server, location | Sets the root directory for handling requests. This will be appended to the prefix and the URI of a request will be appended to this to construct the final file path. |
| types     | types *file*           | types mime.types | http, server, location | Sets the file to retrieve MIME mappings for mapping file name extensions to a MIME type.  |
| listen    | listen *address:port*  | listen 0.0.0.0:8000 | server                 | Sets the endpoint on which the server will accept requests from.

