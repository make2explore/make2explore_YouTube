<!DOCTYPE html>
<html><body>
<?php
/*
  make2explore.com
*/

$servername = "localhost";

// REPLACE with your Database name
$dbname = "XXXXXXXXXXXX";
// REPLACE with Database user
$username = "XXXXXXXXXXXX";
// REPLACE with Database user password
$password = "XXXXXXXXXXXXXX";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, userID, userName, branchName, reading_time FROM LockData ORDER BY id DESC";

echo '<table cellspacing="5" cellpadding="5">
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
</table>
</body>
</html>
