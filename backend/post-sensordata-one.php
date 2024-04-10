<?php
    // Set the timezone to Asia/Manila
    date_default_timezone_set('Asia/Manila');

    // Include the database configuration file
    include_once 'dbconfig.php';

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);

    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // Check if POST data exists
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // Extract POST data
        $postnum = $_POST["postnum"];
        $sensor = $_POST["sensor"];
        $status = $_POST["status"];

        // Get the current date and time
        $currentDateTime = date("Y-m-d H:i:s");

        // SQL to update status and reading time data based on postnum and sensor
        $sql_update_data = "UPDATE sensordata 
                            SET Status = '$status', ReadingTime = '$currentDateTime' 
                            WHERE PostNum = '$postnum' 
                            AND Sensor = '$sensor'";

        // Execute SQL query to update status and reading time data
        if ($conn->query($sql_update_data) === TRUE) {
            echo "Status and ReadingTime updated successfully";
        } else {
            echo "Error updating status and ReadingTime: " . $conn->error;
        }
    } else {
        echo "Invalid request method";
    }

    // Close connection
    $conn->close();
?>
