require 'spec_helper'

describe Sleuthkit do
	it 'should return correct version string' do
		Sleuthkit.version_string.should == "Sleuthkit version #{Sleuthkit::VERSION}"
	end
end
