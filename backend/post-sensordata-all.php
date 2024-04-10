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
        // Extract POST data for each postnum
        $statusforpostnum1 = $_POST["statusforpostnum1"];
        $statusforpostnum2 = $_POST["statusforpostnum2"];
        $statusforpostnum3 = $_POST["statusforpostnum3"];
        $statusforpostnum4 = $_POST["statusforpostnum4"];
        $statusforpostnum5 = $_POST["statusforpostnum5"];

        // Get the current date and time
        $currentDateTime = date("Y-m-d H:i:s");

        // Prepare an array to store the update status data for each postnum
        $updateStatusData = array(
            array("postnum" => "1", "status" => $statusforpostnum1),
            array("postnum" => "2", "status" => $statusforpostnum2),
            array("postnum" => "3", "status" => $statusforpostnum3),
            array("postnum" => "4", "status" => $statusforpostnum4),
            array("postnum" => "5", "status" => $statusforpostnum5)
        );

        // Execute update status query for each postnum
        foreach ($updateStatusData as $data) {
            $postnum = $data["postnum"];
            $status = $data["status"];

            // SQL to update status and reading time data based on postnum
            $sql_update_data = "UPDATE sensordata 
                                SET Status = '$status', ReadingTime = '$currentDateTime' 
                                WHERE PostNum = '$postnum'";

            // Execute SQL query to update status and reading time data
            if ($conn->query($sql_update_data) === TRUE) {
                echo "Status updated successfully for PostNum $postnum <br>";
            } else {
                echo "Error updating status for PostNum $postnum: " . $conn->error . "<br>";
            }
        }
    } else {
        echo "Invalid request method";
    }

    // Close connection
    $conn->close();
?>
