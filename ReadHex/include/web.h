const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <style>
        div {
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        textarea {
            margin-top: 15px;
            width: 50%;
        }
    </style>
</head>
<body>
    
    <div>
        <!-- <input type="file">
        <textarea cols="30" rows="20"></textarea> -->
        <input type="file" name="file" id="file" />
    </div>
    
    <script>
        document.getElementById('file').onchange = function(){
        var file = this.files[0];
        var reader = new FileReader();
        reader.onload = function(progressEvent){
            // By lines
            var lines = this.result.split('\n');
            for(var line = 0; line < lines.length; ){
                //console.log(lines[line])
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function ()
                {
                    if(this.readyState == 4 && this.status == 200)
                    {
                        var str = line.toString() + " =--> "+ lines[line]
                        console.log(str);
                        xhttp.open("GET", "/code?contents=" + str, true); 
                        line++; 
                    }
                    else xhttp.open("GET", "/code", true); 
                    xhttp.send(); 
                }
            }
        };
        reader.readAsText(file);
        };
    </script>
</body>
</html>
    
)====="; 