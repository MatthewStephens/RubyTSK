# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "sleuthkit/version"

Gem::Specification.new do |s|
  s.name        = "sleuthkit"
  s.version     = Sleuthkit::VERSION
  s.platform    = Gem::Platform::RUBY
  s.authors     = ["Matthew Stepehens"]
  s.email       = ["mattbastard@gmail.com"]
  s.homepage    = "https://github.com/MatthewStephens/RubyTSK"
  s.summary     = %q{Ruby bindings for The SleuthKit (TSK) forensics analysis toolkit}
  s.description = %q{Ruby bindings for The SleuthKit (TSK) forensics analysis toolkit.  See http://sleuthkit.org for more information.}

  s.rubyforge_project = "sleuthkit"

  s.files         = `git ls-files`.split("\n")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.require_paths = ["lib"]
	s.add_development_dependency  "rake", "~>0.9.2"
	s.add_development_dependency  "rake-compiler", "~>0.7.1"
	s.add_development_dependency  "rspec", "~>2.0"
  s.add_development_dependency  "bundler", "~>1.1.3"
	s.extensions << "ext/tsk4r/extconf.rb"
end
