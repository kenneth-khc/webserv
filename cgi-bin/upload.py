#!/nix/store/70hc8f915s1wcwnfwndfjg95crmj1659-python3-3.14.0/bin/python3

import os, sys, json, email
from urllib import parse
from http import cookies

def printError(errorCode: int, reasonPhrase: str) -> None:
	print("Content-Type:text/html")
	print(f"Status:{errorCode} {reasonPhrase}")
	print("")
	print(f"<html>{errorCode} {reasonPhrase}</html>")
	sys.exit(1)

def constructFormPath(uploadsDir: str, sid: str) -> str:
	tempFormPath = f"{uploadsDir}/{sid}_form"
	i = 0
	while i < sys.maxsize:
		if i != 0:
			formPath = f"{tempFormPath}{i}.json"
		else:
			formPath = f"{tempFormPath}.json"
		if os.path.exists(formPath) == False:
			break
		i += 1
	if i == sys.maxsize:
		formPath = f"{tempFormPath}.json"
	return formPath

def constructFilePath(uploadsDir: str, sid: str, fileName: str) -> str:
	pos = fileName.rfind(".")
	i = 0

	if pos != -1:
		extension = fileName[pos:]
		tempFilePath = f"{uploadsDir}/{sid}_{fileName[0:pos]}"
	else:
		tempFilePath = f"{uploadsDir}/{sid}_{fileName}"
	while i < sys.maxsize:
		if i != 0:
			filePath = f"{tempFilePath}{i}{extension}"
		else:
			filePath = f"{tempFilePath}{extension}"
		if os.path.exists(filePath) == False:
			break
		i += 1
	if i == sys.maxsize:
		filePath = f"{tempFilePath}{extension}"
	return filePath

def postMethod(uploadsDir: str, c: cookies.SimpleCookie) -> None:
	try:
		bytesToRead = int(os.environ.get('CONTENT_LENGTH'))
	except:
		bytesToRead = 0
	messageBody = bytes(sys.stdin.buffer.read(bytesToRead))
	contentType = os.environ.get('CONTENT_TYPE')
	contentType = bytes(f"Content-Type: {contentType}\n", encoding='utf-8')
	messageObject = email.message_from_bytes(contentType + messageBody)

	if messageObject.get_content_type() == "application/x-www-form-urlencoded":
		uploadDest = constructFormPath(uploadsDir, c['sid'].value)
		form = parse.parse_qs(messageObject.get_payload())
		for key, value in form.items():
			form[key] = value[0]
		try:
			with open(uploadDest, mode='w') as file:
				json.dump(form, file, indent=4)
		except:
			printError(500, "Internal Server Error")
	elif messageObject.get_content_type() == "multipart/form-data":
		try:
			for part in messageObject.get_payload():
				uploadDest = constructFilePath(uploadsDir, c['sid'].value, part.get_filename())

				with open(uploadDest, mode='wb') as file:
					file.write(part.get_payload(decode=True))
		except:
			printError(500, "Internal Server Error")
	else:
		printError(415, "Unsupported Media Type")

	print("Location:http://localhost:8000/pages/form.html")
	print("Status:303 See Other")
	print("")

def getMethod() -> None:
	queryString = os.environ.get("QUERY_STRING")
	form = parse.parse_qs(queryString)

	for key, value in form.items():
		form[key] = value[0]

	output = json.dumps(form, indent=4)

	print("Content-Type:text/html")
	print("")
	print("<html>")
	print("<style>body { background-color: #f4dde7 }</style>")
	print("<h1>I'm from Python CGI!</h1>")
	print("<pre>")
	print(output)
	print("</pre>")
	print("</html>")

if __name__ == "__main__":
	method = os.environ.get('REQUEST_METHOD')
	c = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE'))
	uploadsDir = "uploads"

	if (method == "GET"):
		getMethod()
	elif (method == "POST"):
		postMethod(uploadsDir, c)
	else:
		printError(501, "Not Implemented")
