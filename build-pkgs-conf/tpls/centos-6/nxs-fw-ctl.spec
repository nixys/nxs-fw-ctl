Name:           nxs-fw-ctl
Version:        $$VERSION$$
Release:        $$BUILD_VERSION$$%{?dist}
Summary:        Nixys Framework Control Tools

Group:          Development/Tools
License:        GPL3
URL:            https://nixys.ru
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  make gcc rpm-build rpmdevtools libevent-nxs openssl-devel mysql-devel ncurses-devel libxml2-devel jansson-devel libcurl-devel readline-devel redhat-lsb
Requires:       make gcc rpm-build rpmdevtools libevent-nxs openssl-devel mysql-devel ncurses-devel libxml2-devel jansson-devel libcurl-devel readline-devel redhat-lsb rsync


%description
 Control tools for Nixys Framework.
 It's help to create and manage projects developed on NXS FW.


%prep
%setup -q


%build
make


%install
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/etc
mkdir -p $RPM_BUILD_ROOT/usr/share
cp ./objs/nxs-fw-ctl $RPM_BUILD_ROOT/usr/bin
cp -r ./pkg/etc/nxs-fw-ctl $RPM_BUILD_ROOT/etc
cp -r ./pkg/usr/share/nxs-fw-ctl $RPM_BUILD_ROOT/usr/share

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
/usr/bin/nxs-fw-ctl
/etc/nxs-fw-ctl
/usr/share/nxs-fw-ctl

%changelog
