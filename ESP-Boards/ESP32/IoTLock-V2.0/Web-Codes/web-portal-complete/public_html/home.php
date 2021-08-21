<?php
// We need to use sessions, so you should always start sessions using the below code.
session_start();
// If the user is not logged in redirect to the login page...
if (!isset($_SESSION['loggedin'])) {
	header('Location: index.html');
	exit;
}
?>

<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title>IoT Based Secure Locker</title>
		<link href="style.css" rel="stylesheet" type="text/css">
		<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.1/css/all.css">
      <style>
           table.center {
           margin-left: auto; 
           margin-right: auto;
           }

           table, td {
           border: 1px solid black;
           border-collapse: collapse;
           padding: 12px 15px;
           margin: 25px 0;
           min-width: 100px;
           box-shadow: 0 0 20px rgba(0, 0, 0, 0.15);
           }
           
           table.center tr:nth-child(1) {
           font-weight: bold;
           background-color:orange;
        }
       </style>
	</head>
	<body class="loggedin">
		<nav class="navtop">
			<div>
				<h1>IoT based Secure Locker</h1>
				<a href="profile.php"><i class="fas fa-user-circle"></i>Profile</a>
                <a href="logout.php"><i class="fas fa-sign-out-alt"></i>Logout</a>
			</div>
		</nav>
		<div class="content">
			<h2>User Entry Records</h2>
			<p>Welcome back, <?=$_SESSION['name']?>!</p>
		</div>
<?php
/*
  make2explore.com
*/

$servername = "localhost";

// REPLACE with your Database name
$dbname = "XXXXXXX";
// REPLACE with Database user
$username = "XXXXXXXX";
// REPLACE with Database user password
$password = "xXXXXXXXX";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, userID, userName, branchName, reading_time FROM LockData ORDER BY id DESC";

echo '<table class="center">
      <tr>
        <td>Sr.No</td>
        <td>Users ID</td> 
        <td>Name of User</td> 
        <td>Branch Name</td> 
        <td>Timestamp</td> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $row_id = $row["id"];
        $row_userID = $row["userID"];
        $row_userName = $row["userName"];
        $row_branchName = $row["branchName"];
        $row_reading_time = $row["reading_time"];
      
        $row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 2 hours"));
      
        echo '<tr> 
                <td>' . $row_id . '</td> 
                <td>' . $row_userID . '</td>
                <td>' . $row_userName . '</td>
                <td>' . $row_branchName . '</td> 
                <td>' . $row_reading_time . '</td> 
              </tr>';
    }
    $result->free();
}

$conn->close();
?> 
      
	</body>
</html>