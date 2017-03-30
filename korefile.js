let project = new Project('Compute', __dirname);

project.addFile('Sources/**');
project.setDebugDir('Deployment');

Project.createProject('Kore', __dirname).then((kore) => {
	project.addSubProject(kore);
	resolve(project);
});
