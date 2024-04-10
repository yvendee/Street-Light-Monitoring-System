<?php
    // Include the database configuration file
    include_once 'dbconfig.php';

    // Create connection
    $conn = new mysqli($servername, $username, $password);

    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // Create database
    $sql_create_db = "CREATE DATABASE IF NOT EXISTS $dbname";
    if ($conn->query($sql_create_db) === FALSE) {
        echo "Error creating database: " . $conn->error;
    }

    // Select database
    $conn->select_db($dbname);

    // SQL to create table
    $sql_create_table = "CREATE TABLE IF NOT EXISTS sensordata (
        PostNum TEXT,
        Sensor TEXT,
        Location TEXT,
        Status TEXT,
        ReadingTime TEXT
    )";

    // Execute SQL query to create table
    if ($conn->query($sql_create_table) === TRUE) {
        echo "Table sensordata created successfully";
        
        // Insert mockup data
        $sql_insert_data = "INSERT INTO sensordata (PostNum, Sensor, Location, Status, ReadingTime) VALUES
                            ('1', 'LDR', 'Bagani Campo', '100%', '2024-04-10 09:00:00'),
                            ('2', 'LDR', 'Bagani Gabor', '90%', '2024-04-10 09:15:00'),
                            ('3', 'LDR', 'Oaig-Daya', '80%', '2024-04-10 09:30:00'),
                            ('4', 'LDR', 'Calaoa-an', '70%', '2024-04-10 09:45:00'),
                            ('5', 'LDR', 'Ayudante', '70%', '2024-04-10 09:45:00')";
                            
        if ($conn->query($sql_insert_data) === TRUE) {
            echo "Mockup data inserted successfully";
        } else {
            echo "Error inserting mockup data: " . $conn->error;
        }
    } else {
        echo "Error creating table: " . $conn->error;
    }

    // Close connection
    $conn->close();
?>
