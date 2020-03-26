
<?php  

    $fp = fopen("1.txt","a+");  

    fwrite($fp, date('Y-m-d H:i:s')."*****");  

    fclose($fp);  

?>  
