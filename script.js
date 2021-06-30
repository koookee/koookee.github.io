const swup = new Swup()

// Project filter by programming language
function showFunc(){
  document.getElementById("show").style.color = "#66FCF1";
  document.getElementById("hide").style.color = "#C5C6C7";
  document.getElementById("test1").style.visibility = "visible";
  document.getElementById("test1").style.opacity = 1;
  document.getElementById("test1").style.transition = "opacity 1s linear";
}

function hideFunc(){
  document.getElementById("hide").style.color = "#66FCF1";
  document.getElementById("show").style.color = "#C5C6C7";
  document.getElementById("test1").style.visibility = "hidden";
  document.getElementById("test1").style.opacity = 0.01;
  document.getElementById("test1").style.transition = "visibility 0s 1s, opacity 1s linear";
}


// Project pop ups
function togglePopup(){
  document.getElementById("popup-1").classList.toggle("active");
}
