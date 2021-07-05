const swup = new Swup()
const hasRefreshOnce = false;
const filterIDs = ["All", "JavaScript", "C#", "Java", "Python", "C"];
const popupDictionary = {
  "getStore" : "popup-0",
  "Pomodoro" : "popup-1",
  "PythonImageFilter" : "popup-2",
  "Calculator" : "popup-3",
  "DefendThePortal" : "popup-4",
  "RandomQuoteGenerator" : "popup-5",
  "MarkdownPreviewer" : "popup-6",
  "RoboChicken" : "popup-7",
  "DrumMachine" : "popup-8"
}

// Project filter by programming language
function selectFilter(filterID){
  showFunc(filterID);
  hideFunc(filterID);
}

function showFunc(name){
  document.getElementById(name).style.color = "#66FCF1"; // Changes color of other sub navbar elements
  if (name.localeCompare("All") == 0) {
    for (var i = 0; i < filterIDs.length; i++) {    // Makes every card visible if filter is "All"
      for (var j = 0; j < document.getElementsByClassName(filterIDs[i]).length; j++) {
        document.getElementsByClassName(filterIDs[i])[j].style.visibility = "visible";
        document.getElementsByClassName(filterIDs[i])[j].style.opacity = 1;
        document.getElementsByClassName(filterIDs[i])[j].style.transition = "opacity 1s linear"
        document.getElementsByClassName(filterIDs[i])[j].style.pointerEvents = "all";
      }
    }
  }
  for (var i = 0; i < document.getElementsByClassName(name).length; i++) {
    document.getElementsByClassName(name)[i].style.visibility = "visible";
    document.getElementsByClassName(name)[i].style.opacity = 1;
    document.getElementsByClassName(name)[i].style.transition = "opacity 1s linear"
    document.getElementsByClassName(name)[i].style.pointerEvents = "all";
  }
}

function hideFunc(name){
  filterIDs.forEach((item) => {       // Changes color of other sub navbar elements
    if(item.localeCompare(name) != 0) document.getElementById(item).style.color = "#C5C6C7";
  });
  for (var i = 0; i < filterIDs.length; i++) {     // Filters the cards by the programming language
    for (var j = 0; j < document.getElementsByClassName(filterIDs[i]).length && filterIDs[i] != name && name.localeCompare("All") != 0; j++) {
      //document.getElementsByClassName(filterIDs[i])[j].style.visibility = "hidden";
      document.getElementsByClassName(filterIDs[i])[j].style.pointerEvents = "none";
      document.getElementsByClassName(filterIDs[i])[j].style.opacity = 0;
      document.getElementsByClassName(filterIDs[i])[j].style.transition = "visibility 0s 1s, opacity 1s linear";
    }
  }
}

// Project pop ups
function togglePopup(name){
  document.getElementById(popupDictionary[name]).classList.toggle("active");
}

// Resets the form inputs
function resetForm() {
  setInterval(() => {document.getElementById("gform").reset();}, 1000);
}
