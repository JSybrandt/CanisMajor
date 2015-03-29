#include"Geometry.h"

Geometry::Geometry()
: numVertices(0), numIndexes(0), md3dDevice(0), mVB(0), mIB(0), usesIndexBuffer(false)
{
	indices = nullptr;
	verts = nullptr;
}
 
Geometry::~Geometry()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);

	delete [] verts;
	delete [] indices;
}

void Geometry::init(ID3D10Device* device, D3DXCOLOR color, D3D_PRIMITIVE_TOPOLOGY top)
{
	md3dDevice = device;
	initRasterState();
	this->color = color;
	topology = top;
}

void Geometry::init(ID3D10Device* device, std::string objFile,D3DXCOLOR color)
{
	md3dDevice = device;
	initRasterState();
	this->color = color;
	topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	fstream fin(objFile,std::ios::in);
	//string test;

	if(!fin) throw "THERE WASNT A FILE THERE";

	vector<Vector3> vertices, faces, tempnormals, normals;

	string l,temp;
	float tx,ty,tz;
	float nx,ny,nz;
	while(getline(fin,l))
	{
		stringstream line;
		line<<l;
		line>>temp;
		if(temp == "vn"){
			line >>tx>>ty>>tz;
			tempnormals.push_back(Vector3(tx,ty,tz));
		}
		else if(temp == "v")
		{
			line>>tx>>ty>>tz;
			//_RPT1(0, "position y %f \n", tz);
			vertices.push_back(Vector3(tx,ty,tz));
			normals.push_back(Vector3(0,0,0));//push back an empty normal for use later
		}

		else if(temp == "f")
		{
			//line >> test;
			char * token;
			char * dup = strdup(l.c_str());
			strtok(dup,"// ");
			token = strtok(NULL,"// ");//first face value index
			tx = atoi(token);
			token = strtok(NULL,"// ");//first normal index
			nx = atoi(token);
			token = strtok(NULL,"// ");//second face value index
			ty = atoi(token);
			token = strtok(NULL,"// ");//second normal index
			ny = atoi(token);
			token = strtok(NULL,"// ");//thrid face value index
			tz = atoi(token);
			token = strtok(NULL,"// ");//third normal index
			nz = atoi(token);

			//line>>tx>>ty>>tz;
			faces.push_back(Vector3(tx-1,ty-1,tz-1)); //obj file has 1 based indexes

			//reorganize normals so that first vertex uses the first normal... ect
			normals.at(tx-1) = tempnormals.at(nx-1);
			normals.at(ty-1) = tempnormals.at(ny-1);
			normals.at(tz-1) = tempnormals.at(nz-1);
		}
	}
	fin.close();

	if(vertices.size()>0)
	{
		numVertices = vertices.size();
		verts = new Vertex[numVertices];

		for(int i = 0 ; i < numVertices; i++)
		{
			verts[i].pos = vertices[i];
			verts[i].normal = normals.at(i);

			//normals must be added in according to faces, down below
			verts[i].diffuse = this->color;
			verts[i].spec = D3DXCOLOR(0.2f, 0.2f, 0.2f, 32.0f);
		}

		initVectorBuffer(verts);
	}
	if(faces.size()>0)
	{
		numIndexes = faces.size()*3;
		indices = new DWORD[numIndexes];

		for(int i = 0 ; i < faces.size(); i++)
		{
			indices[3*i] = faces[i].x;
			indices[3*i+1] = faces[i].y;
			indices[3*i+2] = faces[i].z;
		}

		initIndexBuffer(indices);
	}
}


void Geometry::draw(UINT offset)
{
	UINT stride = sizeof(Vertex);

	md3dDevice->IASetPrimitiveTopology(topology);
    md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	
	md3dDevice->RSSetState(g_pRasterState);

	if(usesIndexBuffer)
	{
		md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
		md3dDevice->DrawIndexed(numIndexes, 0, 0);
	}
	else
		md3dDevice->Draw(numVertices,0);
}

void Geometry::initVectorBuffer(Vertex* vertices)
{
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * numVertices;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}

void Geometry::initIndexBuffer(DWORD * indices)
{
	usesIndexBuffer = true;
	D3D10_BUFFER_DESC ibd;
    ibd.Usage = D3D10_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(DWORD)* numIndexes;
    ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Geometry::initRasterState()
{
	rasterState.FrontCounterClockwise = true;
	md3dDevice->CreateRasterizerState(&rasterState,&g_pRasterState);
}