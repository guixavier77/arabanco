#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <locale.h>
#include <ctime>

using namespace std;

#define Max_Contas 100
#define T 100

struct Data{
	int dia;
	int mes;
	int ano;
};


void ParseData(char str[9], Data *Alvo){

	char _dia[3] = {
		str[3],
		str[4],
		0
	};
	char _mes[3] = {
		str[0],
		str[1],
		0
	};
	char _ano[3] = {
		str[6],
		str[7],
		0
	};
	Alvo->dia = atoi(_dia);
	Alvo->mes = atoi(_mes);
	Alvo->ano = atoi(_ano);
	return;
}

class Movimentacao{
	private:
		char Operacao[20];
		float ValorOp;
		
	public:
		void setOperacao( char Op[] ){
			strcpy( this->Operacao, Op);
		}
		char* getOperacao( ){
			return this->Operacao;
		}
		void setValorOp( float V ){
			this->ValorOp = V;
		}
		float getValorOp(){
			return this->ValorOp;
		}
};

class contaCorrente{
	
	private:
		
		char numero_Conta[100];
		char numero_Agencia[100];
		char nome_Titular[100];
		char CPF_Titular[100];
		float saldo;
		int idcMoviment;
		Movimentacao Historico[100];
		Data Atual;
		char Hora[80];
		
	public:	
		contaCorrente(){
			this->idcMoviment = 0;
			this->saldo = 0;
		}
		
		void setNumeroConta ( char numero[] ){
			strcpy(this->numero_Conta, numero);
		}
		
		void setNumeroAgencia ( char numero[] ){
			strcpy( this->numero_Agencia , numero);
		}
		
		void setNomeTitular ( char nome[] ){
			strcpy ( this->nome_Titular, nome );
		}
		
		void setCPF ( char CPF[] ){
			strcpy ( this->CPF_Titular, CPF );
		}
		
		void setSaldo ( float saldo, char Operacao[], Data atual, char hora[] ){
			this->saldo += saldo;
			Historico[idcMoviment].setValorOp(this->saldo);
			Historico[idcMoviment].setOperacao(Operacao);
			this->Atual = atual;
			strcpy( this->Hora, hora );
			this->idcMoviment++;
		}
		
		char* getNumeroConta (){
			return this->numero_Conta;
		}
		
		char* getNumeroAgencia (){
			return this->numero_Agencia;
		}
		
		char* getNomeTitular (){
			return this->nome_Titular;
		}
		
		char* getCPF (){
			return this->CPF_Titular;
		}
		
		float getSaldo (){
			return this->saldo;
		}
		
		void Tela(){
			
			char dateStr[9];
			_strdate(dateStr);
			Data atual;
			ParseData( dateStr, &atual );
			time_t t;
			struct tm *infoTempo;
			char buffer[80];
			time(&t);
			infoTempo = localtime(&t);
			strftime( buffer, 80, "%I:%M", infoTempo );
			
			cout << "\t\t\tEXTRATO BANCÁRIO\t\t\t\n\n\n";
			cout << "\tNome: " << getNomeTitular() << "\t\tAgência/Conta: " << getNumeroAgencia() << "/" << getNumeroConta() << "\n";
			cout << "\tData: " << atual.dia << "/" << atual.mes << "/" << atual.ano << "\t\t\tHorário: " <<  buffer <<"\n";
			cout << "\t\t\tHistórico\t\t\t\n";
			cout << "Data\t\tHorário\t\tLançamento\t\tValor(R$)\t\tSaldo(R$)\n";
			for ( int j = 0; j < idcMoviment; j++ ){
				if( j == 0 ){
				cout << this->Atual.dia << "/"<< this->Atual.mes << "/"<< this->Atual.ano <<"\t"<< this->Hora << "\t\tSaldo inicial\t\t" << this->Historico[j].getValorOp() << "\t\t\t" << this->Historico[j].getValorOp() << endl;
				}
				else{
					cout << this->Atual.dia << "/" << this->Atual.mes << "/" << this->Atual.ano <<"\t" << this->Hora << "\t\t"<< this->Historico[j].getOperacao() <<"\t\t" << this->Historico[j].getValorOp() - this->Historico[j-1].getValorOp() << "\t\t\t" << this->Historico[j].getValorOp() << endl;
				}
			}
		}
		
		void HTML(){
			char dateStr[9];
			_strdate(dateStr);
			Data atual;
			ParseData( dateStr, &atual );
			time_t t;
			struct tm *infoTempo;
			char buffer[80];
			time(&t);
			infoTempo = localtime(&t);
			strftime( buffer, 80, "%I:%M", infoTempo );
			
			ofstream fout;
	
			fout.open( "Extrato.html" ); 
	
			if( !fout ){
				cout << "Erro ao criar o arquivo.";
				return;	
			}
			else{
			
				fout << "<html>\n";
				fout << "<head>\n";
				fout << "<title>";
				fout << "Extrato"; 
				fout << "</title>\n";
				fout << "</head>\n";
				
				fout << "<body style = 'width:40%';>\n";
				fout << "<h1>" << "<center>" << "Extrato Bancário" << "</center>" << "</h1>\n";
				fout << "<p text alingn>";
				fout << "<div align = 'left'><b> Nome: </b>" << getNomeTitular() << "</div><div align = 'right'><b> Conta/Agência: </b>" << getNumeroConta() << "/" << getNumeroAgencia() << "</div>";
				fout << "<div align = 'left'><b> Data: </b>" << atual.dia << "/" << atual.mes << "/" << atual.ano << "</div><div align = 'right'>" << "<b> Horario: </b>" << buffer << "</div>";
				
				fout << "<h3><center>Histórico</center></h3>";
				fout << "<table border = 1, align = 'center'>";
				fout << "<tr>""<td><b>Data</b>&nbsp&nbsp&nbsp&nbsp</td><td><b>Horário</b>&nbsp&nbsp&nbsp&nbsp</td><td><b>Lançamento</b>&nbsp&nbsp&nbsp&nbsp</td><td><b>Valor</b>&nbsp&nbsp&nbsp&nbsp</td><td><b>Saldo atual</b>&nbsp&nbsp&nbsp&nbsp</td></tr>";
				
				for ( int i = 0; i < this->idcMoviment; i++ ){
					
					if ( i == 0 ){
						
						fout << "<tr><td>" << this->Atual.dia << "/" << this->Atual.mes << "/" << this->Atual.ano << "&nbsp&nbsp&nbsp&nbsp</td>" << 
						"<td>" << this->Hora << "&nbsp&nbsp&nbsp&nbsp</td><td>" << this->Historico[i].getOperacao() << "&nbsp&nbsp&nbsp&nbsp</td><td>" << this->Historico[i].getValorOp() <<"&nbsp&nbsp&nbsp&nbsp</td>" <<
						"<td>" << this->Historico[i].getValorOp() << "&nbsp&nbsp&nbsp&nbsp</td></tr>"; 
						
	
					}
					else{
						
						fout << "<tr><td>" << this->Atual.dia << "/" << this->Atual.mes << "/" << this->Atual.ano << "&nbsp&nbsp&nbsp&nbsp</td>" << 
						"<td>" << this->Hora << "&nbsp&nbsp&nbsp&nbsp</td><td>" << this->Historico[i].getOperacao() << "&nbsp&nbsp&nbsp&nbsp</td><td>" << this->Historico[i].getValorOp()  - this->Historico[i-1].getValorOp()  <<"&nbsp&nbsp&nbsp&nbsp</td>" << "<td>" << this->Historico[i].getValorOp() << "&nbsp&nbsp&nbsp&nbsp</td></tr>";
						
					}
					
				}
				
				fout << "</table>";
				fout << "</p>";
		
				fout << "</body>\n";

				fout << "</html>\n";
		
				fout.close();
				
			}
		}
		
};


class Banco:public contaCorrente{
	
	private:
		contaCorrente Contas[Max_Contas];
		int idc;
	public:
		Banco(){
			this->idc = 0;
		}
		
		void addConta( contaCorrente c ){
			if( idc < Max_Contas ){
				Contas[idc] = c;
				idc++;
			}
			else{
				cout << "Não é possível criar contas no momento." << endl;
			}
		}
		
		int selecionarConta( char nConta[], char nAgencia[] ){
			
			int cont = 0, j;
			char nc[100], na[100];
			for ( int i = 0; i < idc; i++ ){
				strcpy(nc , Contas[i].getNumeroConta());
				strcpy( na ,Contas[i].getNumeroAgencia());
				if( strcmp(nc , nConta) == 0 && strcmp(na , nAgencia) == 0 ){
					cont++;
					j = i;
				}
				
			}
			
			if( cont == 1 ){
				return j;
			}
			else{
				return -1;
			}
			
		}
		
		float valorSaldo ( int i ){
			return Contas[i].getSaldo();
		}
		
		void alterarConta ( int i, contaCorrente c ){
			Contas[i] = c;
		}
		
		void excluirConta ( int i ){
			for ( int j = i; j < idc; j++ ){
				Contas[j] = Contas[j+1];
			}
			idc--;
		}
		
		void mostrarContas(){
			
			if( idc == 0 ){
				cout << "Nenhuma conta cadastrada" << endl;
			}
			else{
				for ( int i = 0; i < idc; i++ ){
					cout << i << "   \tConta: " << Contas[i].getNumeroConta() << endl;
					cout << "\tAgencia: " << Contas[i].getNumeroAgencia() << endl;
					cout << "\tTitular: " << Contas[i].getNomeTitular() << endl;
					cout << "\tCPF: " << Contas[i].getCPF() << endl;
					cout << "\tSaldo: " << Contas[i].getSaldo() << " REAIS."<<endl;
					cout << "\t" << endl; 
				}	
			}
		}	

		void consultSaldo(int i) {
			
				cout << "\tConta: " << Contas[i].getNumeroConta() << endl;
				cout << "\tAgencia: " << Contas[i].getNumeroAgencia() << endl;
				cout << "\tTitular: " << Contas[i].getNomeTitular() << endl;
				cout << "\tCPF: " << Contas[i].getCPF() << endl;
				cout << "\tSaldo: R$" << Contas[i].getSaldo() << " REAIS."<<endl;
				cout << "\t" << endl; 
				
		}
		
		void deposito(float ValorDep ,int i){
			
			char dateStr[9];
			_strdate(dateStr);
			Data atual;
			ParseData( dateStr, &atual );
			time_t t;
			struct tm *infoTempo;
			char buffer[80];
			time(&t);
			infoTempo = localtime(&t);
			strftime( buffer, 80, "%I:%M", infoTempo );
			
			Contas[i].setSaldo( ValorDep , "Depósito", atual , buffer );
		
		}
		
		void saque(float ValorSaque, int i){
			char dateStr[9];
			_strdate(dateStr);
			Data atual;
			ParseData( dateStr, &atual );
			time_t t;
			struct tm *infoTempo;
			char buffer[80];
			time(&t);
			infoTempo = localtime(&t);
			Contas[i].setSaldo( -ValorSaque, "Saque", atual , buffer);
		}
		
		void transf(float Valor, int j,  int i){
			char dateStr[9];
			_strdate(dateStr);
			Data atual;
			ParseData( dateStr, &atual );
			time_t t;
			struct tm *infoTempo;
			char buffer[80];
			time(&t);
			infoTempo = localtime(&t);
			
			Contas[i].setSaldo( Valor , "Depósito via transferência", atual, buffer);
			Contas[j].setSaldo( -Valor, "Transferência realizada", atual, buffer); 
			
		}
		
		void mostrarSel(int i){
				cout << i << "   \tConta: " << Contas[i].getNumeroConta() << endl;
				cout << "\tAgencia: " << Contas[i].getNumeroAgencia() << endl;
				cout << "\tTitular: " << Contas[i].getNomeTitular() << endl;
				cout << "\tCPF: " << Contas[i].getCPF() << endl;
				cout << "\tSaldo: " << Contas[i].getSaldo() << " REAIS."<<endl;
				cout << "\t" << endl; 
		}
		
		void extratoTela( int i ){
			Contas[i].Tela();
		}
		
		void extratoHTML ( int i ){
			Contas[i].HTML();		
		}
		
};


	

int main ( ){
	
	setlocale( LC_ALL, "portuguese" );
	
	char nTitular[T], CPF[T], nConta[T], nAgencia[T], Operacao[T], dateStr[9];
	int op, i,j, verif;
	float saldo, VOP, Valor;
	contaCorrente c;
	Banco B;
	Movimentacao M;
	time_t t;
	struct tm *infoTempo;
	char buffer[80];
	
	ofstream fout; // Gravar arquivo
	ifstream fin; // Ler arquivo
	
	fin.open("BancoDados.dat", ios::binary);
	if(!fin){
		cout << "Erro ao inicilizar arquivo";
	} 
	else {
		fin.read((char *)&B,sizeof(B));
		cout << "Arquivo inicializado";
		fin.close();
	}
	
	
	
	do{
		system("cls");
		cout << "*************************************************" << endl;
		cout << "*	  ARABANCO - SEMPRE COM VOCÊ =P  	*" << endl;
		cout << "*************************************************\n\n" ;
		
		cout << "Menu de opções:" << endl;
		cout << "1 - Cadastrar Conta Corrente" << endl;
		cout << "2 - Alterar dados de uma Conta Corrente" << endl;
		cout << "3 - Excluir uma Conta Corrente" << endl;
		cout << "4 - Consultar Saldo" << endl;
		cout << "5 - Fazer Depósito" << endl;
		cout << "6 - Realizar Saque" << endl;
		cout << "7 - Transferir Valores" << endl;
		cout << "8 - Mostrar todas as contas e saldos" << endl;
		cout << "9 - Emitir extrato de conta" << endl;
		cout << "10 - Sair\n\n";
		
		cout << "Escolha uma opção: ";
		cin >> op;
		
		switch( op ){
			
			case 1:
				system("cls");
				
				
				cout << "CADASTRANDO CONTA CORRENTE. \n\n";
				
				cout << "Número da conta: ";
				cin >> nConta;
			
					
				
				cout << "Número da Agência: ";
				cin >> nAgencia;
				
				verif = B.selecionarConta(nConta, nAgencia);
				
				if( verif >= 0 ){
					system("cls");
					cout << "Conta já cadastrada!" << endl;
				}
				else{
				
				
				
				cout << "Nome do titular: ";
				cin >> nTitular;
				
				
				cout << "CPF do titular: ";
				cin >> CPF;

			
				do{
				
				cout << "Saldo inicial: ";
				cin >> saldo;
				
				if ( saldo < 0 ){
					system("cls");
					cout << "Saldo inválido." << endl;
					i = 0;
				}
				else{
					i = 1;
				}
				
				}while( i != 1 );
				
				contaCorrente a;
				
				a.setNumeroConta(nConta);
				a.setNumeroAgencia(nAgencia);
				a.setNomeTitular(nTitular);
				a.setCPF(CPF);
				_strdate(dateStr);
				Data atual;
				ParseData( dateStr, &atual );
				time(&t);
				infoTempo = localtime(&t);
				strftime( buffer, 80, "%I:%M", infoTempo );
				a.setSaldo(saldo, "Saldo inicial", atual, buffer);
				
				B.addConta(a);
				system("cls");
				cout << "Conta corrente cadastrada com sucesso." << endl;
				}
				system("pause");
				break;
			case 2:
				system("cls");
				
				cout << "ALTERANDO DADOS DA CONTA...\n\n";
				
				B.mostrarContas();
				cout << "Informe o número da conta que deseja alterar: ";
				cin >> nConta;
				cout << "Informe a agência da conta que deseja alterar: ";
				cin >> nAgencia;
				
				i = B.selecionarConta( nConta, nAgencia );
				
				if( i < 0 ){
					system("cls");
					cout << "Conta não cadastrada" << endl;
				}
				else{
					system("cls");
					cout << "ALTERANDO DADOS DA CONTA...\n\n";
					cout << "Nome do titular: ";
					cin >> nTitular;
					cout << "CPF do titular: ";
					cin >> CPF;
					
					c.setNumeroAgencia( nAgencia );
					c.setNumeroConta( nConta );
					c.setCPF(CPF);
					c.setNomeTitular( nTitular );
					_strdate(dateStr);
					Data atual;
					ParseData( dateStr, &atual );
					time_t t;
					struct tm *infoTempo;
					char buffer[80];
					time(&t);
					infoTempo = localtime(&t);
					c.setSaldo(B.valorSaldo( i ), " " , atual, buffer);
					
					B.alterarConta( i , c);
					system ("cls");
					

					cout << "Conta alterada com sucesso." << endl;
					
				}
				
				system("pause");
				break;
			case 3:
				system("cls");
				
				cout << "EXCLUINDO CONTA.\n\n";
				
				cout << "Informe o número da conta que deseja excluir: ";
				cin >> nConta;
				cout << "Informe a agência da conta que deseja excluir: ";
				cin >> nAgencia;
				
				i = B.selecionarConta( nConta, nAgencia );
				
				if( i < 0 ){
					system("cls");
					cout << "Conta não cadastrada" << endl;
				}
				else{
					B.excluirConta( i );
					system("cls");
					cout << "Conta excluída com sucesso." << endl;
				}
				
				system("pause");
				break;
			case 4:
				system("cls");
				cout << "CONSULTANDO SALDO...\n";
				cout << "Número: ";
				cin >> nConta;
				cout << "Agência: ";
				cin >> nAgencia;
				
				i = B.selecionarConta(nConta, nAgencia);

				if (i < 0) {
					system("cls");
					cout << "Conta não cadastrada, tente novamente." << endl;
				}
				else {
					cout << "\n" << endl;
					
					B.consultSaldo(i);
					
					system("pause");
				}
				
				system("pause");
				break;
			case 5:
				system("cls");
				
				float ValDep;
				cout << "REALIZANDO DEPÓSITO...\n";
				cout << "Número da conta: ";
				cin >> nConta;
				cout << "Número da Agência: ";
				cin >> nAgencia;
				
				i = B.selecionarConta(nConta, nAgencia);

				if (i < 0) {
					system("cls");
					cout << "Conta não cadastrada, impossível realizar deposito ." << endl;
				}
				
				
				else {
					cout << "\n";
					system ("cls");
					B.mostrarSel(i);
					cout << "Valor á depositar: ";
					cin >> ValDep;
					
					
		
					B.deposito(ValDep,i);
					system("cls");
					cout << "Depósito realizado com sucesso." << endl;
					 					
				}
				
				if (ValDep < 0){
					cout << "Impossível realizar depósito." << endl;
				}
				
				system("pause");
				break;
	
			case 6:
				system("cls");
				
				float ValSaque;

				cout << "REALIZANDO SAQUE...\n";
				cout << "Número da  conta: ";
				cin >> nConta;
				cout << "Número da agência: ";
				cin >> nAgencia;
				
				i = B.selecionarConta(nConta, nAgencia);
				
				if (i < 0) {
					system("cls");
					cout << "Conta não cadastrada, impossível realizar saque ." << endl;
				}
				
				else {
					cout << "\n" << endl;
					system ("cls");
					B.mostrarSel(i);	
					cout << "Valor do saque: ";
					cin >> ValSaque;
					if (ValSaque < 0){
						cout << "Impossível realizar saque." << endl;
						
					}
					else{	
						B.saque(ValSaque, i);
						system("cls");
						cout << "Saque realizado com sucesso." << endl;
						 					
					}
				}
				
				
				
				system("pause");
				break;
			case 7:
				system("cls");
				
				
				cout << "REALIZANDO TRANSFERÊNCIA...\n";
				cout << "CONTA ORIGEM: \n\n";
				cout << "Número da conta: ";
				cin >> nConta;
				cout << "Número da agência: ";
				cin >> nAgencia;
				
				j = B.selecionarConta(nConta, nAgencia);
				
				cout << "CONTA DESTINO: \n\n";
				cout << "Número da conta: ";
				cin >> nConta;
				cout << "Número da agência: ";
				cin >> nAgencia;
				
				i = B.selecionarConta(nConta, nAgencia);
				
				system("cls");			
				
				if (i < 0 || j < 0 ){
					cout << "Dados inválidos, tente novamente\n";
					
				}
				
				else {
					B.mostrarSel(j);
					B.mostrarSel(i);
					
					cout << "Valor da transferência: ";
					cin >> Valor;
					B.transf(Valor,j,i);
					system ("cls");
					cout << "Transferência realizada com sucesso." << endl;
					
				}
				
				if (Valor < 0){
					cout << "Impossível realizar transferência\n";
					
				}
			
			
				system("pause");
				break;
			case 8:
				system("cls");
				
				B.mostrarContas();
			
				
				system("pause");
				break;
			case 9:
				system("cls");
				cout << "EMITINDO EXTRATO...\n";
				cout << "Digite o número da conta: ";
				cin >> nConta;
				cout << "Digite o número da agência: ";
				cin >> nAgencia;
				
				i = B.selecionarConta( nConta, nAgencia );
				
				if( i < 0 ){
					system("cls");
					cout << "Conta não cadastrada" << endl;
				}
				else{
					cout << "Opções de extrato:" << endl;
					cout << "1. Extrato em tela." << endl;
					cout << "2. Extrato em arquivo." << endl;
					cin >> op;
					system("cls");
					if( op == 1 ){
						B.extratoTela(i);
					}
					else if( op == 2){
						system("cls");
						B.extratoHTML(i);
						system("Extrato.html");
					}
					else{
						cout << "Opção inválida." << endl;
					}
				}
				system("pause");
				break;
			case 10:
				system("cls");
				
				fout.open ("BancoDados.dat", ios::binary);
				if(!fout){
					cout << "Erro ao gravar dados";
				}
			
				else{
					fout.write((char* )&B,sizeof(B));
				}
			
				fout.close();
				cout << "Tchau!\n";

				
				
				system("pause");
				break;
			default:
				system("cls");
				
				cout << "Opção inválida, tente novamente.\n";
				
				system("pause");
									
		}
		
	} while ( op != 10 );
		
	
	return 0;
}
