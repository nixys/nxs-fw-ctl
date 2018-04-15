FROM centos:6

ADD packages.nixys.ru.repo /etc/yum.repos.d/packages.nixys.ru.repo
RUN rpm --import http://packages.nixys.ru/centos/repository.gpg.key && \
	yum install -y epel-release && \
	yum install -y nxs-fw-libs-1.14 readline-devel redhat-lsb
RUN rpmdev-setuptree

COPY docker-entrypoint.sh /

ENTRYPOINT ["/docker-entrypoint.sh"]
