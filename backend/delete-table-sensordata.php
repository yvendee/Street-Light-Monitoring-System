<?php
    // Include the database configuration file
    include_once 'dbconfig.php';

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);

    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // SQL to drop table
    $sql_drop_table = "DROP TABLE IF EXISTS sensordata";

    // Execute SQL query to drop table
    if ($conn->query($sql_drop_table) === TRUE) {
        echo "Table sensordata dropped successfully";
    } else {
        echo "Error dropping table: " . $conn->error;
    }

    // Close connection
    $conn->close();
?>
