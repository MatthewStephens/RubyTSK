require 'sleuthkit'
require 'spec_helper'
SAMPLE_DIR="samples"

puts "spec_helper loaded"
describe Sleuthkit.version_string do
	it 'should return correct version string' do
		Sleuthkit.version_string.should match("Sleuthkit Ruby Binding version #{Sleuthkit::VERSION}")
	end
end

describe Sleuthkit::TSK_VERSION do
	it 'should return correct version string' do
		Sleuthkit::TSK_VERSION.should match("3.2.3")
	end
end
