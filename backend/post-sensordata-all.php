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
        $ldrValue1 = $_POST["Sensor1"];
        $ldrValue2 = $_POST["Sensor2"];
        $ldrValue3 = $_POST["Sensor3"];
        $ldrValue4 = $_POST["Sensor4"];
        $ldrValue5 = $_POST["Sensor5"];

        $statusforpostnum1 = $_POST["statusforpostnum1"];
        $statusforpostnum2 = $_POST["statusforpostnum2"];
        $statusforpostnum3 = $_POST["statusforpostnum3"];
        $statusforpostnum4 = $_POST["statusforpostnum4"];
        $statusforpostnum5 = $_POST["statusforpostnum5"];

        // Get the current date and time
        $currentDateTime = date("Y-m-d H:i:s");

        // Prepare an array to store the update status data for each postnum
        $updateData = array(
            array("postnum" => "1", "Sensor" => $ldrValue1, "status" => $statusforpostnum1),
            array("postnum" => "2", "Sensor" => $ldrValue2, "status" => $statusforpostnum2),
            array("postnum" => "3", "Sensor" => $ldrValue3, "status" => $statusforpostnum3),
            array("postnum" => "4", "Sensor" => $ldrValue4, "status" => $statusforpostnum4),
            array("postnum" => "5", "Sensor" => $ldrValue5, "status" => $statusforpostnum5)
        );

        // Execute update query for each postnum
        foreach ($updateData as $data) {
            $postnum = $data["postnum"];
            $ldrValue = $data["Sensor"];
            $status = $data["status"];

            // SQL to update sensor and status data based on postnum
            $sql_update_data = "UPDATE sensordata 
                                SET Sensor = '$ldrValue', Status = '$status', ReadingTime = '$currentDateTime' 
                                WHERE PostNum = '$postnum'";

            // Execute SQL query to update sensor and status data
            if ($conn->query($sql_update_data) === TRUE) {
                echo "Data updated successfully for PostNum $postnum <br>";
            } else {
                echo "Error updating data for PostNum $postnum: " . $conn->error . "<br>";
            }
        }
    } else {
        echo "Invalid request method";
    }

    // Close connection
    $conn->close();
?>
