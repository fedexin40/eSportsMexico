$(document).ready(function(){
	setTeams();

	$('#edit-season-league').change(function(){
		setTeams();
	});
});


function setTeams(){
	var htmlOut = '';
	$.each(teamlist[$('#edit-season-league').val()], function(index, value){
		htmlOut += "<option value='"+index+"'>"+value+"</option>";
	});
	$('#edit-home-team').html(htmlOut);
	$('#edit-away-team').html(htmlOut);
}