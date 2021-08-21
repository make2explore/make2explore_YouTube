<?php

/*
  make2explore.com
*/

$servername = "localhost";

// REPLACE with your Database name
$dbname = "XXXXXXXXXX";
// REPLACE with Database user
$username = "XXXXXXXXXXXXX";
// REPLACE with Database user password
$password = "XXXXXXXXXXXXXX";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "as7fgh5Ab3j2F16";

$api_key= $userID = $userName = $branchName = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $userID = test_input($_POST["userID"]);
        $userName = test_input($_POST["userName"]);
        $branchName = test_input($_POST["branchName"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO LockData (userID, userName, branchName)
        VALUES ('" . $userID . "','" . $userName . "', '" . $branchName . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
