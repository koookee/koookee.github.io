const swup = new Swup()
const hasRefreshOnce = false;
const filterIDs = ["AllFilter", "JavaScriptFilter", "C#Filter", "JavaFilter", "PythonFilter", "CFilter"];

// Project filter by programming language
function selectFilter(filterID){
  showFunc(filterID);
  hideFunc(filterID);
}
function showFunc(name){
  document.getElementById(name).style.color = "#66FCF1";
  /*
  document.getElementById("hide").style.color = "#C5C6C7";
  document.getElementById("test1").style.visibility = "visible";
  document.getElementById("test1").style.opacity = 1;
  document.getElementById("test1").style.transition = "opacity 1s linear";
  */
  /*

  */
}

function hideFunc(name){
  filterIDs.forEach((item) => {
    if(item.localeCompare(name) != 0) document.getElementById(item).style.color = "#C5C6C7";
  });
  /*
  document.getElementById("hide").style.color = "#66FCF1";
  document.getElementById("show").style.color = "#C5C6C7";
  document.getElementById("test1").style.visibility = "hidden";
  document.getElementById("test1").style.opacity = 0.01;
  document.getElementById("test1").style.transition = "visibility 0s 1s, opacity 1s linear";
  */
}



// Project pop ups
function togglePopup(){
  document.getElementById("popup-1").classList.toggle("active");
}

// Refreshes the page so that pop ups can work. For some reason, going from 'home' to 'projects'
// doesn't activate the pop ups unless the page is refreshed at least once.
function ManualRefresh(){
  if(!hasRefreshOnce) setInterval(() => {window.location.reload();}, 1000);
  hasRefreshOnce = true;
}
