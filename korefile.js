var solution = new Solution('Tesselation');
var project = new Project('Tesselation');

project.addFile('Sources/**');
project.setDebugDir('Deployment');

project.addSubProject(Solution.createProject('Kore'));

solution.addProject(project)

return solution;
