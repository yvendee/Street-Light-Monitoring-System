<?php
    // Include the database configuration file
    include_once 'dbconfig.php';

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);

    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // SQL to select all data from sensordata table
    $sql_select_data = "SELECT * FROM sensordata";

    // Execute SQL query
    $result = $conn->query($sql_select_data);

    // Initialize an empty array to store the data
    $data = array();

    // Check if there are any rows returned
    if ($result->num_rows > 0) {
        // Output data of each row
        while($row = $result->fetch_assoc()) {
            // Append each row to the data array
            $data[] = $row;
        }
    } else {
        // If no results found, set data to an empty array
        $data = array();
    }

    // Close connection
    $conn->close();

    // Output the data array as JSON
    header('Content-Type: application/json');
    echo json_encode($data);
?>
