require 'bundler'
require 'rspec/core/rake_task'
require 'rake/extensiontask'
Rake::ExtensionTask.new do |ext| 
		ext.name = 'tsk4r' # name for Ruby C part of sleuthkit module
		ext.ext_dir = 'ext/tsk4r'
		ext.lib_dir = 'lib/tsk4r'
end
Bundler::GemHelper.install_tasks

RSpec::Core::RakeTask.new('spec')
