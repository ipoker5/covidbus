package clases;


public class Sensor {
	Integer idsensor;
	String tipo;
	String nombre;
	Integer iddispositivo;
	
	public Sensor(Integer idsensor, String tipo, String nombre,  Integer iddispositivo) {
		super();
		this.idsensor = idsensor;
		this.tipo = tipo;
		this.nombre = nombre;
		this.iddispositivo = iddispositivo;
	}



	public Integer getIdsensor() {
		return idsensor;
	}

	public void setIdsensor(Integer idsensor) {
		this.idsensor = idsensor;
	}

	public String getTipo() {
		return tipo;
	}

	public void setTipo(String tipo) {
		this.tipo = tipo;
	}

	public String getNombre() {
		return nombre;
	}

	public void setNombre(String nombre) {
		this.nombre = nombre;
	}

	public Integer getIddispositivo() {
		return iddispositivo;
	}

	public void setIddispositivo(Integer iddispositivo) {
		this.iddispositivo = iddispositivo;
	}



	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((iddispositivo == null) ? 0 : iddispositivo.hashCode());
		result = prime * result + ((idsensor == null) ? 0 : idsensor.hashCode());
		result = prime * result + ((nombre == null) ? 0 : nombre.hashCode());
		result = prime * result + ((tipo == null) ? 0 : tipo.hashCode());
		return result;
	}



	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Sensor other = (Sensor) obj;
		if (iddispositivo == null) {
			if (other.iddispositivo != null)
				return false;
		} else if (!iddispositivo.equals(other.iddispositivo))
			return false;
		if (idsensor == null) {
			if (other.idsensor != null)
				return false;
		} else if (!idsensor.equals(other.idsensor))
			return false;
		if (nombre == null) {
			if (other.nombre != null)
				return false;
		} else if (!nombre.equals(other.nombre))
			return false;
		if (tipo == null) {
			if (other.tipo != null)
				return false;
		} else if (!tipo.equals(other.tipo))
			return false;
		return true;
	}

	

}
