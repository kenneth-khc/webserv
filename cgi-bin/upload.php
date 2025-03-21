#!/usr/bin/env php-cgi
<?php
	function printError($errorCode, $reasonPhrase) {
		echo "Content-Type:text/html\n";
		echo "Status:{$errorCode} {$reasonPhrase}\n";
		echo "\n";
		echo "<html>{$errorCode} {$reasonPhrase}</html>\n";
		exit ;
	}

	function constructFormPath($uploadsDir, $sid) {
		$tempFormPath = "{$uploadsDir}/{$sid}_form";
		$i = 0;
		while ($i < PHP_INT_MAX) {
			if ($i != 0)
				$formPath = "{$tempFormPath}{$i}.json";
			else
				$formPath = "{$tempFormPath}.json";
			if (file_exists($formPath) === false)
				break ;
			$i++;
		}
		if ($i === PHP_INT_MAX)
			$formPath = "{$tempFormPath}.json";
		return $formPath;
	}

	function constructFilePath($uploadsDir, $sid, $fileName) {
		$pos = strrpos($fileName, ".");
		$tempFilePath = $fileName;
		$i = 0;

		if ($pos !== false) {
			$extension = substr($fileName, $pos);
			$tempFilePath = "{$uploadsDir}/{$sid}_" . substr($fileName, 0, $pos);
		}
		while ($i < PHP_INT_MAX) {
			if ($i !== 0)
				$filePath = "{$tempFilePath}{$i}{$extension}";
			else
				$filePath = "{$tempFilePath}{$extension}";
			if (file_exists($filePath) === false)
				break ;
			$i++;
		}
		if ($i === PHP_INT_MAX)
			$filePath = "{$tempFilePath}{$extension}";
		return $filePath;
	}

	function postMethod($uploadsDir, $sid) {
		if (strpos($_ENV['CONTENT_TYPE'], ';') !== false)
			$contentType = strstr($_ENV['CONTENT_TYPE'], ';', true);
		else
			$contentType = $_ENV['CONTENT_TYPE'];

		if ($contentType === "application/x-www-form-urlencoded") {
			$uploadDest = constructFormPath("{$_ENV['PWD']}/{$uploadsDir}", $sid);
			$json = json_encode($_POST, JSON_PRETTY_PRINT);
			if (file_put_contents($uploadDest, $json) === false)
				printError(500, "Internal Server Error");
			header("Location:http://localhost:8000/pages/form.html");
			header("Status:303 See Other");
			header("Content-Length:0");
			echo "";
		}
		elseif ($contentType === "multipart/form-data") {
			foreach	($_FILES as $keyOne => $valueOne) {
				$fileNames = array();
				foreach ($valueOne as $keyTwo => $valueTwo) {
					if ($keyTwo === "name") {
						foreach ($valueTwo as $keyThree => $valueThree) {
							$fileName = constructFilePath("{$_ENV['PWD']}/{$uploadsDir}", $sid, $valueThree);
							array_push($fileNames, $fileName);
						}
					}
					elseif ($keyTwo === "tmp_name") {
						foreach ($valueTwo as $keyThree => $valueThree) {
							if (move_uploaded_file($valueThree, $fileNames[$keyThree]) === false)
								printError(500, "Internal Server Error");
						}
					}
				}
			}
			header("Location:http://localhost:8000/pages/form.html");
			header("Status:303 See Other");
			header("Content-Length:0");
			echo "";
		}
	}

	function getMethod($uploadsDir, $sid) {
		$queryString = $_ENV['QUERY_STRING'];
		$uploadDest = constructFormPath("{$_ENV['PWD']}/{$uploadsDir}", $sid);
		$json = json_encode($queryString, JSON_PRETTY_PRINT);
		if (file_put_contents($uploadDest, $json) === false)
			printError(500, "Internal Server Error");
		header("Location:http://localhost:8000/pages/form.html");
		header("Status:303 See Other");
		header("Content-Length:0");
		echo "";
	}

	$method = $_ENV['REQUEST_METHOD'];
	$uploadsDir = $_ENV['X_UPLOADS_DIR'];
	$sid = $_ENV['X_SID'];

	if ($method === "GET")
		getMethod($uploadsDir, $sid);
	elseif ($method === "POST")
		postMethod($uploadsDir, $sid);
	else
		printError(501, "Not Implemented");
?>
