<!DOCTYPE html>
<?php
    require("contact.php");
    $upfolder = "uploads/";
?>
<html>
<head>
    <title>four0four</title>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.6.2/dist/css/bootstrap.min.css" integrity="sha384-xOolHFLEh07PJGoPkLv1IbcEPTNtaed2xpHsD9ESMhqIYd0nLMwNLD69Npy4HI+N" crossorigin="anonymous">
</head>
<body>
    <h1>four0four :(((</h1>
    <h2>We are looking for a fancy GIF for our 404 Page</h2>
    <form action="index.php" method="post" enctype="multipart/form-data">
        <label for="gifimage">The GIF</label>
        <br>
        <input class="form-control" type="file" name="gifimage">
        <input type="hidden" name="scheme" value="file://">
        <br><br>
        <input class="btn btn-primary" type="submit" value="Submit" name="submit">
    </form>
    <br>
<?php
if(isset($_POST["submit"])) {
    $scheme = $_POST["scheme"];
    $tmpFile = $_FILES["gifimage"]["tmp_name"];
    $currentFile = $_FILES["gifimage"]["name"];
    $fileExtension = strtolower(pathinfo($currentFile, PATHINFO_EXTENSION));

    if (!$scheme){
        $scheme = "file://";
    }

    if (mime_content_type($tmpFile) !== "image/gif" || $fileExtension !== "gif") {
        echo "404 NOT GIF!\n";
    }else {
        $new_file_name = bin2hex(random_bytes(32));
        $targetFile = $upfolder . $new_file_name . "." . $fileExtension;
        $success = move_uploaded_file($tmpFile, $targetFile); 
        if ($success) {
            if (preg_match('/phar:\/\//', $scheme)) {
                echo "No phar allowed\n";
                unlink($targetFile);
                exit();
            }

            $targetFile = $scheme . $targetFile;
            if (file_exists(strtolower($targetFile))) {
                echo "File Submitted\n";
            }
        } else {
            echo "Something went seriously wrong\n";
        }
    }
}
?>
</body>
</html>
