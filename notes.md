## Timeouts
|CGI|Error Request|Header Complete|Header Correct|Body|Body Complete|Body Correct|Output|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|❌|❌|✅|✅|❌|-|-|KeepAlive Timeout|
|❌|❌|✅|❌|❌|-|-|Bad Request (close)|
|❌|❌|❌|-|❌|-|-|Client Header Timeout|
|❌|❌|✅|✅|✅ (chunked)|❌|✅|Lingering Timeout|
|❌|❌|✅|✅|✅ (content-length)|❌|✅|Lingering Timeout|
|✅|❌|✅|✅|✅ (chunked)|❌|✅|Client Body Timeout|
|✅|❌|✅|✅|✅ (content-length)|❌|✅|Client Body Timeout|
|✅|❌|✅|✅|✅ (chunked)|❌|❌|Bad Request (close)|
|❌|✅|✅|✅|❌|-|-|KeepAlive Timeout|
|❌|✅|✅|❌|❌|-|-|Bad Request (close)|
|❌|✅|✅|✅|✅ (chunked)|❌|✅|Lingering Timeout|
|❌|✅|✅|✅|✅ (chunked)|❌|❌|Not Allowed (close)|
|✅|✅|✅|✅|✅ (chunked)|❌|✅|Client Body Timeout|
|✅|✅|✅|✅|✅ (content-length)|❌|✅|Client Body Timeout|
|✅|✅|✅|✅|✅ (chunked)|❌|❌|Bad Request (close)|

Not Implemented (close) for Unrecognized Transfer-Encoding