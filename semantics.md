# Basic Core HTTP Semantics
## Core Rules
|Rule|Value|Description|Check|
|-|-|-|:-:|
|ALPHA|%x41-5A / %x61-7A|A-Z / a-z|✅(std::isalpha)|
|BIT|"0" / "1"|||
|BWS|OWS|Bad White Space and Optional White Space||
|CHAR|%x01-7F|ASCII Character||
|CR|%x0D|Carriage Return|✅|
|CRLF|CR LF|CR Line Feed|❕|
|DIGIT|%x30-39|0 - 9|✅(std::isdigit)|
|DQUOTE|%x22|Double Quote|✅|
|HEXDIG|DIGIT / "A" / "B" / "C" / "D" / "E" / "F"||✅(std::isxdigit)|
|HTAB|%x09|Horizontal Tab|✅|
|LF|%x0A|Line Feed|✅|
|LWSP|*( WSP / CRLF WSP )|Linear White Space||
|obs-text|%x80-FF||✅|
|OCTET|%x00-FF||❕|
|OWS|*( SP / HTAB )||❕|
|pct-encoded|"%" HEXDIG HEXDIG||✅|
|RWS|1*( SP / HTAB )|Required White Space||
|SP|%x20||✅|
|sub-delims|"!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="||✅|
|token|1*tchar||✅|
|tchar|"!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." / "^" / "_" / "`" / "\\" / "\|" / "~" / DIGIT / ALPHA||✅|
|unreserved|ALPHA / DIGIT / "-" / "." / "_" / "~"||✅|
|VCHAR|%x21-7E|Visible Character|✅(std::isgraph)|
|WSP|SP / HTAB||✅|

## URI Rules
|Rule|Value|Description|Check|
|-|-|-|:-:|
|URI|scheme ":" hier-part [ "?" query ][ "#" fragment ]|||
|scheme|ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )||✅|
|hier-part|"//" authority path-abempty / path-absolute / path-rootless / path-empty|abempty: begins with "/" or is empty / absolute: begins with "/" but not "//" / rootless: begins with a segment / empty: zero characters|✅|
|authority|host [ ":" port ]||✅|
|host|IP-literal / IPv4address / reg-name||✅|
|IP-literal|"[" ( IPv6address / IPvFuture ) "]"||✅|
|IPv6address|6( h16 ":" ) ls32 / "::" 5( h16 ":" ) ls32 / [ h16 ] "::" 4( h16 ":" ) ls32 / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32 / [ *4( h16 ":" ) h16 ] "::" ls32 / [ *5( h16 ":" ) h16 ] "::" h16 / [ *6( h16 ":" ) h16 ] "::"||✅|
|ls32|( h16 ":" h16 ) / IPv4address|Least-significant 32 bits of address|❕|
|h16|1*4HEXDIG|16 bits of address represented in hexadecimal|✅|
|IPv4address|dec-octet "." dec-octet "." dec-octet "." dec-octet||✅|
|dec-octet|DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35|0-9 / 10 - 99 / 100-199 / 200-249 / 250-255|✅|
|reg-name|*( unreserved / pct-encoded / sub-delims )|✅|
|port|*DIGIT||✅|
|path-abempty|*( "/" segment )|Begins with "/" or is empty|
|segment|*pchar||✅|
|pchar|unreserved / pct-encoded / sub-delims / ":" / "@"||✅|
|path-absolute|"/" [ segment-nz *( "/" segment ) ]||✅|
|segment-nz|1*pchar||✅|
|path-rootless|segment-nz *( "/" segment )||✅|
|path-empty|0pchar||✅|
|query|*( pchar / "/" / "?" )||✅|
|fragment|*( pchar / "/" / "?" )||✅|
|URI-reference|URI / relative-ref|||
|relative-ref|relative-part [ "?" query ] [ "#" fragment ]|||
|relative-part|"//" authority path-abempty / path-absolute / path-noscheme / path-empty|||
|path-noscheme|segment-nz-nc *( "/" segment )||✅|
|segment-nz-nc|1*( unreserved / pct-encoded / sub-delims / "@" )|Non-zero-length segment without any colon ":"|✅|
|absolute-URI|scheme ":" hier-part [ "?" query ]||✅|

## General Message Rules
|Rule|Value|Description|Check|
|-|-|-|:-:|
|HTTP-message|start-line CRLF *( field-line CRLF ) CRLF [ message-body ]||❕|
|start-line|request-line / status-line|||
|request-line|method SP request-target SP HTTP-version||✅|
|method|token||❕|
|request-target|origin-form / absolute-form / authority-form / asterisk-form||✅|
|origin-form|absolute-path [ "?" query ]||✅|
|asbolute-path|1*( "/" segment )||✅|
|absolute-form|absolute-URI||✅|
|authority-form|host ":" port||✅|
|asterisk-form|"*"||✅|
|HTTP-version|"HTTP/" DIGIT "." DIGIT||❕|
|status-line|HTTP-version SP status-code SP [ reason-phrase ]|||
|status-code|3DIGIT|||
|reason-phrase|1*( HTAB / SP / VCHAR / obs-text )|||
|field-line|field-name ":" OWS field-value OWS||✅|
|field-name|token||❕|
|field-value|*field-content||✅|
|field-content|field-vchar [ 1*( SP / HTAB / field-vchar ) field-vchar]||✅|
|field-vchar|VCHAR / obs-text||✅|
|message-body|*OCTET||❌|

## Headers
### Connection
|Rule|Value|Description|Check|
|-|-|-|:-:|
|Connection|[ connection-option *( OWS "," OWS connection-option ) ]|||
|connection-option|token|||

### Content
|Rule|Value|Description|Check|
|-|-|-|:-:|
|Content-Language|[ language-tag *( OWS "," OWS language-tag ) ]|||
|Content-Length|1*DIGIT|||
|Content-Type|media-type|||

### Date
|Rule|Value|Description|Check|
|-|-|-|:-:|
|Date|HTTP-date||✅|
|HTTP-date|IMF-fixdate / obs-date||✅|
|IMF-fixdate|day-name "," SP date1 SP time-of-day SP GMT||✅|
|day-name|%x4D.6F.6E / %x54.75.65 / %x57.65.64 / %x54.68.75 / %x46.72.69 / %x53.61.74 / %x53.75.6E|Mon / Tue / Wed / Thu / Fri / Sat / Sun|✅|
|date1|day SP month SP year||✅|
|day|2DIGIT||✅|
|month|%x4A.61.6E / %x46.65.62 / %x4D.61.72 / %x41.70.72 / %x4D.61.79 / %x4A.75.6E / %x4A.75.6C / %x41.75.67 / %x53.65.70 / %x4F.63.74 / %x4E.6F.76 / %x44.65.63|Jan / Feb / Mar / Apr / May / Jun / Jul / Aug / Sep / Oct / Nov / Dec|✅|
|year|4DIGIT||✅|
|time-of-day|hour ":" minute ":" second||✅|
|hour|2DIGIT||✅|
|minute|2DIGIT||✅|
|second|2DIGIT||✅|
|GMT|%x47.4D.54|GMT|✅|
|obs-date|rfc850-date / asctime-date||✅|
|rfc850-date|day-name-1 "," SP date2 SP time-of-day SP GMT||✅|
|day-name-1|%x4D.6F.6E.64.61.79 / %x54.75.65.73.64.61.79 / %x57.65.64.6E.65.73.64.61.79 / %x54.68.75.72.73.64.61.79 / %x46.72.69.64.61.79 / %x53.61.74.75.72.64.61.79 / %x53.75.6E.64.61.79|Monday / Tuesday / Wednesday / Thursday / Friday / Saturday / Sunday|✅|
|date2|day "-" month "-" 2DIGIT||✅|
|asctime-date|day-name SP date3 SP time-of-day SP year||✅|
|date3|month SP ( 2DIGIT / ( SP 1DIGIT ) )||✅|

### Host
|Rule|Value|Description|Check|
|-|-|-|:-:|
|Host|host [ ":" port ]||❕|

### Last-Modified
|Rule|Value|Description|Check|
|-|-|-|:-:|
|Last-Modified|HTTP-date|||

### Server
|Rule|Value|Description|Check|
|-|-|-|:-:|
|Server|product *( RWS ( product / comment ) )|||

### User-Agent
|Rule|Value|Description|Check|
|-|-|-|:-:|
|User-Agent|product *( RWS ( product / comment ) )|||
|product|token [ "/" product-version ]|||
|product-version|token|||
|comment|"(" *( ctext / quoted-pair / comment ) ")"|||
|ctext|HTAB / SP / %x21-27 / %x2A-5B / %x5D-7E / obs-text|HTAB / SP / '!'-''' / '*'-'[' / ']'-'~' / obs-text||
|quoted-pair|"\\" ( HTAB / SP / VCHAR / obs-text )|||

### Cookies
|Rule|Value|Description|Check|
|-|-|-|:-:|
|Set-Cookie|set-cookie-string|||
|set-cookie-string|cookie-pair *( ";" SP cookie-av )|||
|cookie-pair|cookie-name "=" cookie-value||✅|
|cookie-name|token||❕|
|cookie-value|*cookie-octet / ( DQUOTE *cookie-octet DQUOTE )||✅|
|cookie-octet|%x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E|US-ASCII characters excluding CTLs, whitespace, DQUOTE, comma, semicolon, and backslash|✅|
|cookie-av|expires-av / max-age-av / domain-av / path-av / secure-av / httponly-av|||
|expires-av|"Expires=" Date|||
|max-age-av|"Max-Age=" non-zero-digit *DIGIT|In practice, both expires-av and max-age-av are limited to dates representable by the user agent||
|non-zero-digit|%x31-39|digits 1 through 9||
|domain-av|"Domain=" domain-value|||
|domain-value|<subdomain>|||
|path-av|"Path=" path-value|||
|path-value|<any CHAR except CTLs or ";">|||
|secure-av|"Secure"|||
|httponly-av|"HttpOnly"|||
|cookie-header|"Cookie:" OWS cookie-string OWS||❕|
|cookie-string|cookie-pair *( ";" SP cookie-pair )||❕|

### Entity Tag
|Rule|Value|Description|Check|
|-|-|-|:-:|
|ETag|entity-tag||✅|
|entity-tag|[ weak ] opaque-tag||✅|
|opaque-tag|DQUOTE *etagc DQUOTE||✅|
|etagc|%x21 / %x23-7E / obs-text|VCHAR except double quotes, plus obs-text|✅|

### If-None-Match
|Rule|Value|Description|Check|
|-|-|-|:-:|
|If-None-Match|"*" / #entity-tag||❕|

### If-Modified-Since
|Rule|Value|Description|Check|
|-|-|-|:-:|
|If-Modified-Since|HTTP-date||❕|