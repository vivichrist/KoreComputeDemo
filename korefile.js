var solution = new Solution('Compute');
var project = new Project('Compute');

project.addFile('Sources/**');
project.setDebugDir('Deployment');

project.addSubProject(Solution.createProject('Kore'));

solution.addProject(project)

return solution;
