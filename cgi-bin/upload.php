#!/nix/store/d7cf70kmr5j9p2mpyz65wxbxkp94fvsr-php-with-extensions-8.2.27/bin/php-cgi
<?php
	function printError($errorCode, $reasonPhrase) {
		echo "Content-Type:text/html";
		echo "Status:{$errorCode} {$reasonPhrase}";
		echo "";
		echo "<html>{$errorCode} {$reasonPhrase}</html>";
		exit(1);
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
		else
			$tempFilePath = "{$uploadsDir}/{$sid}_{$fileName}";
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

	function postMethod($uploadsDir) {
		if (strpos($_ENV['CONTENT_TYPE'], ';') !== false)
			$contentType = strstr($_ENV['CONTENT_TYPE'], ';', true);
		else
			$contentType = $_ENV['CONTENT_TYPE'];

		if ($contentType === "application/x-www-form-urlencoded") {
			$uploadDest = constructFormPath($uploadsDir, $_COOKIE['sid']);
			$json = json_encode($_POST, JSON_PRETTY_PRINT);
			if (file_put_contents($uploadDest, $json) === false)
				printError(500, "Internal Server Error");
		}
		elseif ($contentType === "multipart/form-data") {
			foreach	($_FILES as $keyOne => $valueOne) {
				$fileNames = array();
				foreach ($valueOne as $keyTwo => $valueTwo) {
					if ($keyTwo === "name") {
						foreach ($valueTwo as $keyThree => $valueThree)
							array_push($fileNames, $valueThree);
					}
					elseif ($keyTwo === "tmp_name") {
						foreach ($valueTwo as $keyThree => $valueThree) {
							$fileName = constructFilePath($uploadsDir, $_COOKIE['sid'], $fileNames[$keyThree]);
							if (move_uploaded_file($valueThree, $fileName) === false)
								printError(500, "Internal Server Error");
						}
					}
				}
			}
		}
		header("Location:http://localhost:8000/pages/form.html");
		header("Status:303 See Other");
		echo "";
	}

	function getMethod($uploadsDir) {
		$queryString = $_ENV['QUERY_STRING'];
		$uploadDest = constructFormPath($uploadsDir, $_COOKIE['sid']);

		$queries = explode('&', $queryString);
		$queryPairs = array();
		foreach ($queries as $query) {
			$queryPair = explode('=', $query);
			$queryPairs[$queryPair[0]] = $queryPair[1];
		}
		$json = json_encode($queryPairs, JSON_PRETTY_PRINT);

		header("Content-Type:text/html");
		echo "<html>";
		echo "<style>body { background-color: #f4dde7 }</style>";
		echo "<h1>I'm from PHP-CGI!</h1>";
		echo "<pre>";
		echo $json;
		echo "</pre>";
		echo "</html>";
	}

	$method = $_ENV['REQUEST_METHOD'];
	$uploadsDir = "{$_ENV['DOCUMENT_ROOT']}/{$_ENV['X_UPLOADS_DIR']}";

	if ($method === "GET")
		getMethod($uploadsDir);
	elseif ($method === "POST")
		postMethod($uploadsDir);
	else
		printError(501, "Not Implemented");
?>
