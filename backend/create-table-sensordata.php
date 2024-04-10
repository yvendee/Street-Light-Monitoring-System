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
    } else {
        echo "Error creating table: " . $conn->error;
    }

    // Close connection
    $conn->close();
?>
