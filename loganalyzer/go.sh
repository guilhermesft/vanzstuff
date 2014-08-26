#Script auxiliar utilizado para criar toda a estrutura de diretóricos e arquivos utilizados para testar e rodar a aplicação
#Author: José Guilherme Vanz <guilherme.sft@gmail.com>

#função para criação dos diretórios utilizados pela aplicação para simular diversos servidores
function create_dirs()
{
	#verifica a número de pasta que deve ser criada
	if [ $1 -gt 0 ];then
		loop=1
		for i in `seq 1 $1`
		do
			mkdir -p servers/server-$i
		done
	fi
}


#criar os diretórios necessários para simular 4 servidores
create_dirs 4
echo "Diretórios criados com sucesso"

