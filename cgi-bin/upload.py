#!/usr/bin/env python3

import os, sys, json, email
from email.message import EmailMessage
from urllib import parse

def printError(errorCode: int, reasonPhrase: str) -> None:
	print("Content-Type:text/html")
	print(f"Status:{errorCode} {reasonPhrase}")
	print("")
	print(f"<html>{errorCode} {reasonPhrase}</html>")
	sys.exit()

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
	tempFilePath = fileName
	i = 0

	if pos != -1:
		extension = fileName[pos:]
		tempFilePath = f"{uploadsDir}/{sid}_{fileName[0:pos]}"
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

def postMethod(uploadsDir: str, sid: str) -> None:
	contentType = os.environ.get('CONTENT_TYPE')
	messageObject = email.message_from_string(f"Content-Type: {contentType}\n{messageBody}")

	if messageObject.get_content_type() == "application/x-www-form-urlencoded":
		uploadDest = constructFormPath(uploadsDir, sid)
		form = parse.parse_qs(messageObject.get_payload())
		for key, value in form.items():
			form[key] = value[0]

		try:
			with open(uploadDest, mode='w') as file:
				json.dump(form, file, indent=4)
			print("Location:http://localhost:8000/pages/form.html")
			print("Status:303 See Other")
			print("Content-Length:0")
			print("")
		except:
			printError(500, "Internal Server Error")
	elif messageObject.get_content_type() == "multipart/form-data":
		try:
			for part in messageObject.get_payload():
				uploadDest = constructFilePath(uploadsDir, sid, part.get_filename())

				with open(uploadDest, mode='w') as file:
					file.write(part.get_payload())
			print("Location:http://localhost:8000/pages/form.html")
			print("Status:303 See Other")
			print("Content-Length:0")
			print("")
		except:
			printError(500, "Internal Server Error")
	else:
		printError(415, "Unsupported Media Type")

def getMethod(uploadsDir: str, sid: str) -> None:
	queryString = os.environ.get("QUERY_STRING")
	uploadDest = constructFormPath(uploadsDir, sid)
	form = parse.parse_qs(queryString)

	for key, value in form.items():
		form[key] = value[0]

	try:
		with open(uploadDest, mode='w') as file:
			json.dump(form, file, indent=4)
		print("Location:http://localhost:8000/pages/form.html")
		print("Status:303 See Other")
		print("Content-Length:0")
		print("")
	except:
		printError(500, "Internal Server Error")

if __name__ == "__main__":
	messageBody = sys.stdin.read()
	method = os.environ.get('REQUEST_METHOD')
	uploadsDir = os.environ.get('X_UPLOADS_DIR')
	sid = os.environ.get('X_SID')

	if (method == "GET"):
		getMethod(uploadsDir, sid)
	elif (method == "POST"):
		postMethod(uploadsDir, sid)
	else:
		printError(501, "Not Implemented")
